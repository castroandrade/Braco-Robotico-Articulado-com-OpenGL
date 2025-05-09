# Projeto Braço Robótico com OpenGL

## 📝 Descrição do Projeto
Simulação interativa de um braço robótico articulado em 3D desenvolvido em C++ com OpenGL/GLUT. O projeto demonstra transformações geométricas hierárquicas e controle via teclado.

## 📂 Estrutura de Diretórios
```
/
├── include/GL/
│   ├── freeglut_ext.h
│   ├── freeglut_std.h
│   ├── freeglut.h
│   └── glut.h
├── lib/x64/
│   ├── libfreeglut_static.a
│   └── libfreeglut.a
├── output/
│   └── freeglut.dll
├── robo_arm.cpp        # Código fonte principal
├── robo_arm.exe        # Executável gerado
├── launch.json         # Configuração do VS Code
└── tasks.json          # Tarefas de compilação
```

## ⚙️ Pré-requisitos
- Compilador GCC (MinGW no Windows)

## 🛠️ Compilação
Execute no terminal:
```bash
g++ robo_arm.cpp -o robo_arm.exe -I ./include -L ./lib/x64 -lfreeglut -lopengl32 -lglu32
```

## 🏃 Execução
Após compilar, execute:
```bash
./robo_arm.exe
```
Certifique-se que **freeglut.dll** esteja no mesmo diretório do executável ou no **PATH** do sistema.

## ⌨️ Controles
- **Movimento da Base:** W (frente), S (trás), A (esquerda), D (direita)

- **Seleção de Segmentos:**
  - 1: Braço
  - 2: Antebraço

- **Rotação:**
  - Q/E: Rotação X
  - R/F: Rotação Y
  - T/G: Rotação Z

- **Garra:**
  - Y: abrir
  - U: fechar

- **Câmera:**
  - I/K: Girar lentamente
  - J/L: Girar rapidamente
  - Scroll do Mouse: Zoom in/out

- **Sair:**
  - ESC

## 📚 Bibliotecas Utilizadas
- OpenGL (core)
- GLUT (FreeGLUT)
- GLU

## 📝 Notas
- No Windows, as DLLs necessárias devem estar no mesmo diretório do executável
- Projeto desenvolvido para Windows 64-bit
- Requer hardware com suporte a OpenGL 2.1+

## ✉️ Autores
- José Henrique Castro Andrade
- Priscila de Araújo Andrade

Este projeto foi desenvolvido como parte da disciplina de Computação Gráfica.
