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

