//Dupla
//José Henrique Castro Andrade
//Priscila Araújo

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    float comprimento;
    float largura;
    float altura;
    float anguloX;
    float anguloY;
    float anguloZ;
    float cor[3];
} SegmentoRobo;

typedef struct {
    float abertura;
    float comprimento;
    float largura;
    float cor[3];
} Garra;

// Variáveis globais
SegmentoRobo base, braco, antebraco;
Garra garra;
int segmentoAtual = 0; // 0: base, 1: braço, 2: antebraço, 3: garra
float posX = 0.0f, posY = 0.0f;
float anguloVisao = 45.0f;

// Inicializa os parâmetros do braço robótico
// Inicializa os parâmetros do braço robótico
void inicializarBraco() {
    // Base - Azul profundo
    // Base 
    base.comprimento = 2.0f;
    base.largura = 2.0f;
    base.altura = 0.5f;
    base.anguloX = 0.0f;
    base.anguloY = 0.0f;
    base.anguloZ = 0.0f;
    base.cor[0] = 0.1f; base.cor[1] = 0.2f; base.cor[2] = 0.4f;

    // Braço - Azul médio
    // Braço 
    braco.comprimento = 3.0f;
    braco.largura = 0.8f;
    braco.altura = 0.8f;
    braco.anguloX = 0.0f;
    braco.anguloY = 0.0f;
    braco.anguloZ = 0.0f;
    braco.cor[0] = 0.2f; braco.cor[1] = 0.3f; braco.cor[2] = 0.6f;

    // Antebraço - Azul claro
    // Antebraço 
    antebraco.comprimento = 2.5f;
    antebraco.largura = 0.6f;
    antebraco.altura = 0.6f;
    antebraco.anguloX = 0.0f;
    antebraco.anguloY = 0.0f;
    antebraco.anguloZ = 0.0f;
    antebraco.cor[0] = 0.3f; antebraco.cor[1] = 0.5f; antebraco.cor[2] = 0.8f;

    // Garra - Dourado/amarelo para contraste
    // Garra 
    garra.comprimento = 0.5f;
    garra.largura = 0.4f;
    garra.abertura = 45.0f;
    garra.cor[0] = 0.9f; garra.cor[1] = 0.7f; garra.cor[2] = 0.2f;
}
// Função para desenhar um cubo (usada para os segmentos)
void desenharCubo(float comprimento, float largura, float altura, float cor[3]) {
    glColor3fv(cor);

    glBegin(GL_QUADS);
        // Face frontal
        glVertex3f(-largura/2, -altura/2, comprimento/2);
        glVertex3f(largura/2, -altura/2, comprimento/2);
        glVertex3f(largura/2, altura/2, comprimento/2);
        glVertex3f(-largura/2, altura/2, comprimento/2);

        // Face traseira
        glVertex3f(-largura/2, -altura/2, -comprimento/2);
        glVertex3f(largura/2, -altura/2, -comprimento/2);
        glVertex3f(largura/2, altura/2, -comprimento/2);
        glVertex3f(-largura/2, altura/2, -comprimento/2);

        // Face superior
        glVertex3f(-largura/2, altura/2, -comprimento/2);
        glVertex3f(largura/2, altura/2, -comprimento/2);
        glVertex3f(largura/2, altura/2, comprimento/2);
        glVertex3f(-largura/2, altura/2, comprimento/2);

        // Face inferior
        glVertex3f(-largura/2, -altura/2, -comprimento/2);
        glVertex3f(largura/2, -altura/2, -comprimento/2);
        glVertex3f(largura/2, -altura/2, comprimento/2);
        glVertex3f(-largura/2, -altura/2, comprimento/2);

        // Face lateral esquerda
        glVertex3f(-largura/2, -altura/2, -comprimento/2);
        glVertex3f(-largura/2, altura/2, -comprimento/2);
        glVertex3f(-largura/2, altura/2, comprimento/2);
        glVertex3f(-largura/2, -altura/2, comprimento/2);

        // Face lateral direita
        glVertex3f(largura/2, -altura/2, -comprimento/2);
        glVertex3f(largura/2, altura/2, -comprimento/2);
        glVertex3f(largura/2, altura/2, comprimento/2);
        glVertex3f(largura/2, -altura/2, comprimento/2);
    glEnd();
}

// Função para desenhar a garra
void desenharGarra() {
    // Desenha a parte fixa da garra
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, garra.comprimento/2);
    desenharCubo(garra.comprimento, garra.largura, garra.largura/3, garra.cor);
    glPopMatrix();

    // Desenha os dois dedos da garra
    glPushMatrix();
    glTranslatef(garra.largura/2, 0.0f, garra.comprimento);
    glRotatef(garra.abertura, 0.0f, 1.0f, 0.0f);
    desenharCubo(garra.comprimento, garra.largura/3, garra.largura/2, garra.cor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-garra.largura/2, 0.0f, garra.comprimento);
    glRotatef(-garra.abertura, 0.0f, 1.0f, 0.0f);
    desenharCubo(garra.comprimento, garra.largura/3, garra.largura/2, garra.cor);
    glPopMatrix();
}

// Função para desenhar todo o braço robótico
void desenharBracoRobotico() {
    glPushMatrix();

    // Move a base para a posição atual
    glTranslatef(posX, posY, 0.0f);

    // Desenha a base
    glPushMatrix();
    desenharCubo(base.comprimento, base.largura, base.altura, base.cor);
    glPopMatrix();

    // Rotaciona e desenha o braço
    glPushMatrix();
    glTranslatef(0.0f, base.altura/2 + braco.altura/2, 0.0f);
    glRotatef(braco.anguloX, 1.0f, 0.0f, 0.0f);
    glRotatef(braco.anguloY, 0.0f, 1.0f, 0.0f);
    glRotatef(braco.anguloZ, 0.0f, 0.0f, 1.0f);
    desenharCubo(braco.comprimento, braco.largura, braco.altura, braco.cor);

    // Rotaciona e desenha o antebraço
    glTranslatef(0.0f, 0.0f, braco.comprimento/2 + antebraco.comprimento/2);
    glRotatef(antebraco.anguloX, 1.0f, 0.0f, 0.0f);
    glRotatef(antebraco.anguloY, 0.0f, 1.0f, 0.0f);
    glRotatef(antebraco.anguloZ, 0.0f, 0.0f, 1.0f);
    desenharCubo(antebraco.comprimento, antebraco.largura, antebraco.altura, antebraco.cor);

    // Desenha a garra
    glTranslatef(0.0f, 0.0f, antebraco.comprimento/2);
    desenharGarra();

    glPopMatrix();
    glPopMatrix();
}

// Função para exibir instruções na tela - VERSÃO MELHORADA
// Função para exibir instruções na tela 
void exibirInstrucoes() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    float y = glutGet(GLUT_WINDOW_HEIGHT) - 20;
    float alturaDeLinha = 18;

    glColor3f(0.2f, 0.5f, 1.0f); 

    // Título
    glRasterPos2f(20, y);
    char titulo[] = "CONTROLES DO BRACO ROBOTICO:";
    for (int i = 0; titulo[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, titulo[i]);
    }
    y -= alturaDeLinha + 5;

    glColor3f(0.9f, 0.9f, 1.0f);

    // Seleção de segmentos
    glRasterPos2f(20, y);
    char selecao[] = "Selecionar segmento:  [1] Base  [2] Braco  [3] Antebraco  [4] Garra";
    for (int i = 0; selecao[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, selecao[i]);
    }
    y -= alturaDeLinha;

    // Movimento
    glRasterPos2f(20, y);
    char movimento[] = "Mover base:  [W] Frente  [S] Tras  [A] Esquerda  [D] Direita";
    for (int i = 0; movimento[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, movimento[i]);
    }
    y -= alturaDeLinha;

    // Rotação X
    glRasterPos2f(20, y);
    char rotX[] = "Rotacao X:  [Q] Anti-horario  [E] Horario";
    for (int i = 0; rotX[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, rotX[i]);
    }
    y -= alturaDeLinha;

    // Rotação Y
    glRasterPos2f(20, y);
    char rotY[] = "Rotacao Y:  [R] Anti-horario  [F] Horario";
    for (int i = 0; rotY[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, rotY[i]);
    }
    y -= alturaDeLinha;

    // Rotação Z
    glRasterPos2f(20, y);
    char rotZ[] = "Rotacao Z:  [T] Anti-horario  [G] Horario";
    for (int i = 0; rotZ[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, rotZ[i]);
    }
    y -= alturaDeLinha;

    // Garra
    glRasterPos2f(20, y);
    char garraCtrl[] = "Controle da garra:  [Y] Abrir  [U] Fechar";
    for (int i = 0; garraCtrl[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, garraCtrl[i]);
    }
    y -= alturaDeLinha;

    // Visão
    glRasterPos2f(20, y);
    char visao[] = "Mudar camera:  [I][K] Girar lentamente  [J][L] Girar rapidamente";
    for (int i = 0; visao[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, visao[i]);
    }
    y -= alturaDeLinha;

    // Sair
    glRasterPos2f(20, y);
    char sair[] = "Sair:  [ESC]";
    for (int i = 0; sair[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, sair[i]);
    }
    y -= alturaDeLinha + 5;

    // Informação sobre o segmento atual
    glColor3f(0.0f, 1.0f, 0.5f);
    glRasterPos2f(20, y);

    char segmentoAtualStr[50];
    const char* segmentos[] = {"Base", "Braco", "Antebraco", "Garra"};
    sprintf(segmentoAtualStr, "SEGMENTO ATUAL: %s", segmentos[segmentoAtual]);

    for (int i = 0; segmentoAtualStr[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, segmentoAtualStr[i]);
    }

    // Retorna ao estado anterior da matriz
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// Função de display principal
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Configura a visão da cena
    gluLookAt(7.0f * sin(anguloVisao * M_PI / 180.0f), 
          4.0f, 
          7.0f * cos(anguloVisao * M_PI / 180.0f),
          0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f);

    // Desenha o braço robótico
    desenharBracoRobotico();

    // Exibe as instruções
    exibirInstrucoes();

    glutSwapBuffers();
}

// Função para redimensionar a janela
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Função para tratar eventos do teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Seleção de segmentos
        case '1': segmentoAtual = 0; break;
        case '2': segmentoAtual = 1; break;
        case '3': segmentoAtual = 2; break;
        case '4': segmentoAtual = 3; break;

        // Movimento da base
        case 'a': posX -= 0.1f; break;
        case 'd': posX += 0.1f; break;
        case 'w': posY += 0.1f; break;
        case 's': posY -= 0.1f; break;

        // Rotação X
        case 'q': 
            if (segmentoAtual == 0) base.anguloX -= 5.0f;
            else if (segmentoAtual == 1) braco.anguloX -= 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloX -= 5.0f;
            break;
        case 'e': 
            if (segmentoAtual == 0) base.anguloX += 5.0f;
            else if (segmentoAtual == 1) braco.anguloX += 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloX += 5.0f;
            break;

        // Rotação Y
        case 'r': 
            if (segmentoAtual == 0) base.anguloY -= 5.0f;
            else if (segmentoAtual == 1) braco.anguloY -= 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloY -= 5.0f;
            break;
        case 'f': 
            if (segmentoAtual == 0) base.anguloY += 5.0f;
            else if (segmentoAtual == 1) braco.anguloY += 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloY += 5.0f;
            break;

        // Rotação Z
        case 't': 
            if (segmentoAtual == 0) base.anguloZ -= 5.0f;
            else if (segmentoAtual == 1) braco.anguloZ -= 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloZ -= 5.0f;
            break;
        case 'g': 
            if (segmentoAtual == 0) base.anguloZ += 5.0f;
            else if (segmentoAtual == 1) braco.anguloZ += 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloZ += 5.0f;
            break;

        // Abertura da garra
        case 'y': garra.abertura = fmin(garra.abertura + 5.0f, 90.0f); break;
        case 'u': garra.abertura = fmax(garra.abertura - 5.0f, 0.0f); break;

        // Mudar ângulo de visão
        case 'i': anguloVisao += 5.0f; break;
        case 'k': anguloVisao -= 5.0f; break;
        case 'j': anguloVisao += 15.0f; break;
        case 'l': anguloVisao -= 15.0f; break;

        // Sair do programa
        case 27: exit(0); break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Braco Robotico Articulado - Computacao Grafica");

    inicializarBraco();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}