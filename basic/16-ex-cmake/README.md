# Solução para Erro com wxGraphicsContext

O erro ocorre porque `wxGraphicsContext::Create(wxDC&)` não existe no wxWidgets 3.3. Você precisa usar o tipo específico de DC ou usar `wxGCDC` como wrapper.

## 🔧 Correção do Código

Há várias formas de corrigir. Vou mostrar as melhores:

### Solução 1: Usar wxGCDC (Recomendado)

```cpp
class SolidCirclePainter final : public IBallPainter {
public:
  explicit SolidCirclePainter(bool antialias = false)
      : m_antialias(antialias) {}

  void Draw(wxDC &dc, const Ball &b) override {
    if (!m_antialias) {
      // Desenho simples sem antialias
      dc.SetBrush(wxBrush(b.color));
      dc.SetPen(wxPen(b.color));
      dc.DrawCircle((int)std::lround(b.x), (int)std::lround(b.y), b.radius);
    } else {
      // CORREÇÃO: Use wxGCDC como wrapper
      wxGCDC gcdc(dc);
      
      // Agora pode acessar o GraphicsContext
      wxGraphicsContext* gc = gcdc.GetGraphicsContext();
      if (gc) {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetBrush(wxBrush(b.color));
        gc->SetPen(wxPen(b.color));
        const double d = 2.0 * b.radius;
        gc->DrawEllipse(b.x - b.radius, b.y - b.radius, d, d);
        // NÃO delete gc - é gerenciado pelo wxGCDC
      }
    }
  }

private:
  bool m_antialias = false;
};
```

### Solução 2: Verificar Tipo do DC

```cpp
void Draw(wxDC &dc, const Ball &b) override {
  if (!m_antialias) {
    dc.SetBrush(wxBrush(b.color));
    dc.SetPen(wxPen(b.color));
    dc.DrawCircle((int)std::lround(b.x), (int)std::lround(b.y), b.radius);
  } else {
    // Tenta criar contexto baseado no tipo do DC
    wxGraphicsContext* gc = nullptr;
    
    // Verifica se é um tipo específico de DC
    if (auto* wdc = wxDynamicCast(&dc, wxWindowDC)) {
      gc = wxGraphicsContext::Create(*wdc);
    } else if (auto* mdc = wxDynamicCast(&dc, wxMemoryDC)) {
      gc = wxGraphicsContext::Create(*mdc);
    }
    
    if (gc) {
      gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
      gc->SetBrush(wxBrush(b.color));
      gc->SetPen(wxPen(b.color));
      const double d = 2.0 * b.radius;
      gc->DrawEllipse(b.x - b.radius, b.y - b.radius, d, d);
      delete gc; // AGORA sim precisa deletar
    } else {
      // Fallback: desenho sem antialias
      dc.SetBrush(wxBrush(b.color));
      dc.SetPen(wxPen(b.color));
      dc.DrawCircle((int)std::lround(b.x), (int)std::lround(b.y), b.radius);
    }
  }
}
```

### Solução 3: Mais Simples - Desenhar Direto com wxGCDC no OnPaint

Esta é a **melhor solução** pois já usa Graphics Context desde o início:

```cpp
class BouncingBallPanel : public wxPanel {
  // ... resto do código ...

private:
  void OnPaint(wxPaintEvent &) {
    wxAutoBufferedPaintDC pdc(this);
    pdc.SetBackground(*wxWHITE_BRUSH);
    pdc.Clear();

    // CORREÇÃO: Use wxGCDC para ter antialias em tudo
    wxGCDC dc(pdc);

    for (const auto &w : m_balls) {
      w.painter->Draw(dc, w.ball);
    }
  }
  
  // ... resto do código ...
};
```

Com essa mudança, você pode simplificar o `SolidCirclePainter`:

```cpp
class SolidCirclePainter final : public IBallPainter {
public:
  explicit SolidCirclePainter(bool antialias = false)
      : m_antialias(antialias) {}

  void Draw(wxDC &dc, const Ball &b) override {
    if (m_antialias) {
      // Como já estamos usando wxGCDC, podemos acessar o GraphicsContext
      wxGraphicsContext* gc = nullptr;
      
      // Tenta obter o contexto se for um wxGCDC
      if (auto* gcdc = wxDynamicCast(&dc, wxGCDC)) {
        gc = gcdc->GetGraphicsContext();
      }
      
      if (gc) {
        gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
        gc->SetBrush(wxBrush(b.color));
        gc->SetPen(wxPen(b.color));
        const double d = 2.0 * b.radius;
        gc->DrawEllipse(b.x - b.radius, b.y - b.radius, d, d);
        return;
      }
    }
    
    // Fallback: desenho simples
    dc.SetBrush(wxBrush(b.color));
    dc.SetPen(wxPen(b.color));
    dc.DrawCircle((int)std::lround(b.x), (int)std::lround(b.y), b.radius);
  }

private:
  bool m_antialias = false;
};
```

## 📝 Código Completo Corrigido (Solução Recomendada)## 📋 Principais Mudanças

### 1. **OnPaint - Usa wxGCDC**
```cpp
// ANTES:
void OnPaint(wxPaintEvent &) {
  wxAutoBufferedPaintDC dc(this);
  // ...

// DEPOIS:
void OnPaint(wxPaintEvent &) {
  wxAutoBufferedPaintDC pdc(this);
  pdc.SetBackground(*wxWHITE_BRUSH);
  pdc.Clear();
  
  // Usa wxGCDC para suportar GraphicsContext
  wxGCDC dc(pdc);
  // ...
```

### 2. **SolidCirclePainter - Verifica Tipo do DC**
```cpp
// ANTES (errado):
auto gc = wxGraphicsContext::Create(dc); // ❌ Não existe
delete gc; // ❌ Erro de tipo

// DEPOIS (correto):
wxGraphicsContext* gc = nullptr;
if (auto* gcdc = wxDynamicCast(&dc, wxGCDC)) {
  gc = gcdc->GetGraphicsContext();
}
// NÃO delete gc - pertence ao wxGCDC ✅
```

## 🎯 Por que as Mudanças Funcionam

| Problema | Solução | Motivo |
|----------|---------|--------|
| `Create(wxDC&)` não existe | Usar `wxGCDC` wrapper | wxGCDC encapsula qualquer DC |
| `delete gc` erro de tipo | Não deletar | gc pertence ao wxGCDC |
| Sem antialias | `wxGCDC(pdc)` | Habilita GraphicsContext |

## 🚀 Compile e Execute

```bash
# Limpe
rm -rf build CMakeFiles CMakeCache.txt

# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Compile
cmake --build build

# Execute
./build/app
```

## ✅ Resumo

As correções garantem:
- ✅ Compatibilidade com wxWidgets 3.3
- ✅ Antialias funciona corretamente
- ✅ Sem memory leaks
- ✅ Código portável

Agora deve compilar sem erros! 🎉
