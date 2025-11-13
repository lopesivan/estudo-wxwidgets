Para “aumentar” (ampliar) a área VISÍVEL do plano você tem duas opções distintas – escolha a que melhor se encaixa no que você quer:

--------------------------------------------------
1. Aumentar a JANELA (mais pixels, mesma escala)
--------------------------------------------------
Basta mudar os defines:

```c
#define LARGURA 1200   // ou 1920, etc.
#define ALTURA  700    // ou 1080, etc.
#define ESCALA  50.0f  // continua 50 px/unidade
```

- Você ganha mais “terreno” sem alterar a grade.  
- Útil quando a tela física comporta janelas maiores.

--------------------------------------------------
2. Manter a JANELA e alterar a ESCALA (zoom out)
--------------------------------------------------
Deixe `LARGURA` e `ALTURA` iguais, mas diminua `ESCALA`:

```c
#define LARGURA 800
#define ALTURA  600
#define ESCALA  25.0f   // 25 px = 1 unidade → DOBRO de área visível
```

- 25 px/unidade mostra o DOBRO de espaço (de –16 a +16 aprox.).  
- Valores menores que 50 dão zoom OUT; valores maiores (75, 100) dão zoom IN.

--------------------------------------------------
3. Combinar ambos (janela maior + escala menor)
--------------------------------------------------
```c
#define LARGURA 1200
#define ALTURA  700
#define ESCALA  20.0f   // muito “zoom out”
```

--------------------------------------------------
4. Fazer ZOOM DINÂMICO (roda do mouse)
--------------------------------------------------
Se quiser ampliar/reduzir em tempo de execução, mantenha `ESCALA` como variável global:

```c
float escala = 50.0f;
```

No evento `ALLEGRO_EVENT_MOUSE_AXES` capture a roda:

```c
else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
    if (evento.mouse.dz != 0) {          // roda girou
        escala += evento.mouse.dz * 3.0f; // ajuste a velocidade
        if (escala < 5.0f)  escala = 5.0f;
        if (escala > 200.0f) escala = 200.0f;
    }
}
```

Troque todos os lugares que usavam `ESCALA` pela variável `escala`.

--------------------------------------------------
Resumo rápido
--------------------------------------------------
- Janela maior ➜ muda `LARGURA/ALTURA`.  
- Ver mais plano ➜ diminua `ESCALA` (zoom out).  
- Ver menos plano (mais detalhe) ➜ aumente `ESCALA` (zoom in).  
- Quer ajuste em tempo real ➜ torne `escala` variável e use a roda do mouse.

Pronto! É só escolher a combinação que melhor servir ao seu projeto.



Técnica passo-a-passo para **ocupar mais área útil** (mais “metros quadrados” do plano) **sem aumentar a janela fisicamente**, ou seja, **aumentar a precisão / densidade de informação** que cabe na tela:

--------------------------------------------------
1. Entenda o que realmente controla “área visível”
--------------------------------------------------
```
área horizontal visível = LARGURA / ESCALA
área vertical visível   = ALTURA  / ESCALA
```
- Diminuir `ESCALA` → mais unidades cabem na mesma quantidade de pixels.  
- Aumentar `ESCALA` → menos unidades, zoom-in.  

--------------------------------------------------
2. Reduza ESCALA até quase o limite de “pixelização”
--------------------------------------------------
Exemplo prático:  
```
LARGURA = 1200 px  
ESCALA  = 10 px/un → 1200/10 = 120 unidades de largura  
ESCALA  = 5 px/un  → 240 unidades (DOBRO de área!)
```
Limite: quando 1 unidade → 1 pixel, linhas começam a ficar “serrilhadas”.  
Teste empírico: `ESCALA = 8.0f` ou `6.0f` ainda é legível; abaixo disso use anti-aliasing (`al_draw_line` com espessura 0 ou 1 já suaviza).

--------------------------------------------------
3. Use sub-divisões de grade menores
--------------------------------------------------
Se antes traçava linha a cada 1 unidade, agora traçe a cada 0.5 ou 0.2:  
```c
float passo = 0.2f;
for (float i = -xmax; i <= xmax; i += passo) {
    int px = LARGURA/2 + (int)(i * ESCALA);
    al_draw_line(px, 0, px, ALTURA, cor_fina, 0);
}
```
Isso **não ocupa mais pixels**, mas dá a sensação de “precisão” mesmo com `ESCALA` pequena.

--------------------------------------------------
4. Aproveite os “cantões” da tela: mude a origem
--------------------------------------------------
Se seu gráfico só existe no 1.º quadrante, **mova a origem para o canto inferior-esquerdo** em vez do centro:  
```c
// mundo → tela
*px = (int)(mx * ESCALA);
*py = ALTURA - (int)(my * ESCALA);   // Y cresce para cima
```
Assim **100 % dos pixels** mostram x≥0, y≥0; nada é “desperdiçado” com áreas negativas vazias.

--------------------------------------------------
5. Zoom dinâmico + pan: máxima área sob demanda
--------------------------------------------------
- **Roda do mouse** → altera `ESCALA` em tempo real (já mostrado antes).  
- **Arraste com botão direito** → desloca offset (`origemX`, `origemY`).  
Com isso você **visualiza infinitos “pedaços”** do plano sem janela maior.

Exemplo de offset:
```c
float origemX = 0.0f, origemY = 0.0f;  // unidades, não pixels

void mundo_para_tela(float mx, float my, int *px, int *py){
    *px = (int)((mx - origemX) * ESCALA);
    *py = ALTURA - (int)((my - origemY) * ESCALA);
}
```
Ao arrastar o mouse com o botão direito você soma/subtrai em `origemX`/`origemY`.

--------------------------------------------------
6. Resumo da técnica “mais área, mesma tela”
--------------------------------------------------
1. Diminua `ESCALA` até 5-10 px/un.  
2. Desenhe grade fina (0.2 em 0.2).  
3. (Opcional) Mova origem para o canto que seu gráfico usa.  
4. (Opcional) Implemente zoom com roda + pan com botão direito.  

Aplicando só os passos 1 e 2 você **dobra ou triplica** a quantidade de unidades visíveis **sem aumentar um único pixel** da janela.

