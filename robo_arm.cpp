//Dupla
//José Henrique Castro Andrade
//Priscila de Araújo Andrade

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

// Protótipos das funções
void inicializarBraco();
void desenharCubo(float comprimento, float largura, float altura, float cor[3]);
void desenharEsfera(float raio, float cor[3], int slices, int stacks);
void desenharCilindro(float raio, float altura, float cor[3], int slices);
void desenharBaseComArticulacao();
void desenharBracoComArticulacao();
void desenharAntebracoComArticulacao();
void desenharGarra();
void desenharBracoRobotico();
void exibirInstrucoes();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void mouseWheel(int wheel, int direction, int x, int y);

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
    float anguloX;
    float anguloY;
    float anguloZ;
} Garra;

SegmentoRobo base, braco, antebraco;
Garra garra;
int segmentoAtual = 0; // 0: base, 1: braço, 2: antebraço, 3: garra
float posX = 0.0f, posY = 0.0f;
float anguloVisao = 45.0f;
float zoom = 1.0f; // Fator de zoom inicial
float distanciaCamera = 7.0f; // Distância inicial da câmera

// Inicializa os parâmetros do braço robótico
void inicializarBraco() {
    // Base
    base.comprimento = 2.0f;
    base.largura = 2.0f;
    base.altura = 0.5f;
    base.anguloX = 0.0f;
    base.anguloY = 0.0f;
    base.anguloZ = 0.0f;
    base.cor[0] = 0.1f; base.cor[1] = 0.2f; base.cor[2] = 0.4f;

    // Braço
    braco.comprimento = 4.0f;
    braco.largura = 0.8f;
    braco.altura = 0.8f;
    braco.anguloX = 0.0f;
    braco.anguloY = 0.0f;
    braco.anguloZ = 0.0f;
    braco.cor[0] = 0.2f; braco.cor[1] = 0.3f; braco.cor[2] = 0.6f;

    // Antebraço
    antebraco.comprimento = 3.0f;
    antebraco.largura = 0.6f;
    antebraco.altura = 0.6f;
    antebraco.anguloX = 0.0f;
    antebraco.anguloY = 0.0f;
    antebraco.anguloZ = 0.0f;
    antebraco.cor[0] = 0.3f; antebraco.cor[1] = 0.5f; antebraco.cor[2] = 0.8f;

    // Garra
    garra.comprimento = 0.5f;
    garra.largura = 0.4f;
    garra.abertura = 45.0f;
    garra.cor[0] = 0.9f; garra.cor[1] = 0.7f; garra.cor[2] = 0.2f;
}

// Função para desenhar uma esfera (articulação)
void desenharEsfera(float raio, float cor[3], int slices = 16, int stacks = 16) {
    glColor3fv(cor);
    glutSolidSphere(raio, slices, stacks);
}

// Função para desenhar um cilindro (articulação)
void desenharCilindro(float raio, float altura, float cor[3], int slices = 16) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    glColor3fv(cor);
    gluCylinder(quadric, raio, raio, altura, slices, 1);
    gluDeleteQuadric(quadric);
}

// Função para desenhar a base com articulação
void desenharBaseComArticulacao() {
    // Desenha a base
    glPushMatrix();
    glTranslatef(0.0f, base.altura/2, 0.0f);
    desenharCubo(base.comprimento, base.largura, base.altura, base.cor);
    
    // Articulação na parte superior da base
    glTranslatef(0.0f, base.altura/2, 0.0f);
    float corArticulacao[3] = {0.7f, 0.7f, 0.7f}; // Cor cinza metálico
    desenharEsfera(braco.largura * 0.6f, corArticulacao);
    glPopMatrix();
}

// Função para desenhar o braço com articulação
void desenharBracoComArticulacao() {
    // Desenha o braço
    desenharCubo(braco.comprimento, braco.largura, braco.altura, braco.cor);
    
    // Articulação na extremidade do braço
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, braco.comprimento);
    float corArticulacao[3] = {0.7f, 0.7f, 0.7f};
    desenharEsfera(antebraco.largura * 0.6f, corArticulacao);
    glPopMatrix();
}

// Função para desenhar o antebraço com articulação
void desenharAntebracoComArticulacao() {
    // Desenha o antebraço
    desenharCubo(antebraco.comprimento, antebraco.largura, antebraco.altura, antebraco.cor);
    
    // Articulação na extremidade do antebraço (para a garra)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, antebraco.comprimento);
    float corArticulacao[3] = {0.7f, 0.7f, 0.7f};
    desenharEsfera(garra.largura * 0.8f, corArticulacao);
    glPopMatrix();
}

// Função para desenhar um cubo (usada para os segmentos)
void desenharCubo(float comprimento, float largura, float altura, float cor[3]) {
    glColor3fv(cor);

    glBegin(GL_QUADS);
        // Face frontal (z = comprimento)
        glVertex3f(-largura/2, -altura/2, comprimento);
        glVertex3f(largura/2, -altura/2, comprimento);
        glVertex3f(largura/2, altura/2, comprimento);
        glVertex3f(-largura/2, altura/2, comprimento);

        // Face traseira (z = 0)
        glVertex3f(-largura/2, -altura/2, 0.0f);
        glVertex3f(largura/2, -altura/2, 0.0f);
        glVertex3f(largura/2, altura/2, 0.0f);
        glVertex3f(-largura/2, altura/2, 0.0f);

        // Faces laterais
        glVertex3f(largura/2, -altura/2, 0.0f);
        glVertex3f(largura/2, -altura/2, comprimento);
        glVertex3f(largura/2, altura/2, comprimento);
        glVertex3f(largura/2, altura/2, 0.0f);

        glVertex3f(-largura/2, -altura/2, 0.0f);
        glVertex3f(-largura/2, -altura/2, comprimento);
        glVertex3f(-largura/2, altura/2, comprimento);
        glVertex3f(-largura/2, altura/2, 0.0f);

        // Faces superior e inferior
        glVertex3f(-largura/2, altura/2, 0.0f);
        glVertex3f(largura/2, altura/2, 0.0f);
        glVertex3f(largura/2, altura/2, comprimento);
        glVertex3f(-largura/2, altura/2, comprimento);

        glVertex3f(-largura/2, -altura/2, 0.0f);
        glVertex3f(largura/2, -altura/2, 0.0f);
        glVertex3f(largura/2, -altura/2, comprimento);
        glVertex3f(-largura/2, -altura/2, comprimento);
    glEnd();
}

// Função para desenhar a garra
void desenharGarra() {
    // Parte fixa da garra
    glPushMatrix();
    desenharCubo(garra.comprimento, garra.largura, garra.largura/3, garra.cor);
    
    // Dedo direito
    glPushMatrix();
    glTranslatef(garra.largura/2, 0.0f, garra.comprimento);
    glRotatef(garra.abertura, 0.0f, 1.0f, 0.0f);
    desenharCubo(garra.comprimento/2, garra.largura/3, garra.largura/2, garra.cor);
    glPopMatrix();
    
    // Dedo esquerdo
    glPushMatrix();
    glTranslatef(-garra.largura/2, 0.0f, garra.comprimento);
    glRotatef(-garra.abertura, 0.0f, 1.0f, 0.0f);
    desenharCubo(garra.comprimento/2, garra.largura/3, garra.largura/2, garra.cor);
    glPopMatrix();
    
    glPopMatrix();
}

// Função para desenhar todo o braço robótico com articulações
void desenharBracoRobotico() {
    glPushMatrix();
    
    // Move a base para a posição atual
    glTranslatef(posX, posY, 0.0f);
    
    // Desenha a base com articulação superior
    desenharBaseComArticulacao();
    
    // Braço - rotação na articulação superior da base
    glPushMatrix();
    // Posiciona no topo da base (centro da articulação)
    glTranslatef(0.0f, base.altura + braco.largura*0.6f, 0.0f);
    
    // Aplica rotações no ponto de junção
    glRotatef(braco.anguloX, 1.0f, 0.0f, 0.0f);
    glRotatef(braco.anguloY, 0.0f, 1.0f, 0.0f);
    glRotatef(braco.anguloZ, 0.0f, 0.0f, 1.0f);
    
    // Desenha o braço com articulação
    desenharBracoComArticulacao();
    
    // Antebraço - rotação na articulação do braço
    // Move para a extremidade do braço (centro da articulação)
    glTranslatef(0.0f, 0.0f, braco.comprimento + antebraco.largura*0.6f);
    
    // Aplica rotações no ponto de junção
    glRotatef(antebraco.anguloX, 1.0f, 0.0f, 0.0f);
    glRotatef(antebraco.anguloY, 0.0f, 1.0f, 0.0f);
    glRotatef(antebraco.anguloZ, 0.0f, 0.0f, 1.0f);
    
    // Desenha o antebraço com articulação
    desenharAntebracoComArticulacao();
    
    // Garra - posiciona na articulação do antebraço
    glTranslatef(0.0f, 0.0f, antebraco.comprimento + garra.largura*0.8f);
    desenharGarra();
    
    glPopMatrix();
    glPopMatrix();
}

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
    char selecao[] = "Selecionar segmento:  [1] Braco  [2] Antebraco";
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

    char zoomInst[] = "Zoom: [Scroll do Mouse]";
    glRasterPos2f(20, y);
    for (int i = 0; zoomInst[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, zoomInst[i]);
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
    const char* segmentos[] = {"Nenhum", "Braco", "Antebraco"};
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

    // Configura a visão da cena com zoom
    gluLookAt(distanciaCamera * sin(anguloVisao * M_PI / 180.0f), 
              4.0f * zoom, 
              distanciaCamera * cos(anguloVisao * M_PI / 180.0f),
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
        // Seleção de segmentos (agora apenas 1 e 2)
        case '1': segmentoAtual = 1; break;  // Braço
        case '2': segmentoAtual = 2; break;  // Antebraço

        // Movimento da base (sempre ativo, não precisa selecionar)
        case 'a': posX -= 0.1f; break;
        case 'd': posX += 0.1f; break;
        case 'w': posY += 0.1f; break;
        case 's': posY -= 0.1f; break;

        // Rotação X (para braço e antebraço)
        case 'q': 
            if (segmentoAtual == 1) braco.anguloX -= 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloX -= 5.0f;
            break;
        case 'e': 
            if (segmentoAtual == 1) braco.anguloX += 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloX += 5.0f;
            break;

        // Rotação Y (para braço e antebraço)
        case 'r': 
            if (segmentoAtual == 1) braco.anguloY -= 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloY -= 5.0f;
            break;
        case 'f': 
            if (segmentoAtual == 1) braco.anguloY += 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloY += 5.0f;
            break;

        // Rotação Z (para braço e antebraço)
        case 't': 
            if (segmentoAtual == 1) braco.anguloZ -= 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloZ -= 5.0f;
            break;
        case 'g': 
            if (segmentoAtual == 1) braco.anguloZ += 5.0f;
            else if (segmentoAtual == 2) antebraco.anguloZ += 5.0f;
            break;

        // Abertura da garra (sempre ativa, não precisa selecionar)
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

// Controle de Zoom
void mouse(int button, int state, int x, int y) {
    if (button == 3 && state == GLUT_DOWN) { // Scroll up
        zoom *= 0.9f;
        distanciaCamera *= 0.9f;
    } else if (button == 4 && state == GLUT_DOWN) { // Scroll down
        zoom *= 1.1f;
        distanciaCamera *= 1.1f;
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

    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}