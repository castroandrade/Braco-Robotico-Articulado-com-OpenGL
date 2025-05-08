// robo_arm.cpp: Simulação de um braço robótico articulado usando OpenGL, GLU e GLUT

#include <GL/glut.h>
#include <cstdio>
#include <cmath>


// Estruturas de dados
struct SegmentoRobo {
    float comprimento, largura, altura;
    float anguloX, anguloY, anguloZ;
    float cor[3];
};

struct Garra {
    float abertura;       // ângulo de abertura
    float comprimento;
    float largura;
    float cor[3];
};

// Variáveis globais
SegmentoRobo baseSeg = { 1.0f, 0.4f, 0.4f, 0, 0, 0, {0.6f, 0.6f, 0.6f} };
SegmentoRobo braco    = { 1.2f, 0.2f, 0.2f, 0, 0, 0, {0.8f, 0.2f, 0.2f} };
SegmentoRobo antebraco= { 1.0f, 0.18f,0.18f,0, 0, 0, {0.2f, 0.8f, 0.2f} };
Garra garra           = { 20.0f, 0.3f, 0.05f, {0.2f, 0.2f, 0.8f} };

int selecionado = 0; // 0=base,1=braço,2=antebraço,3=garra
float cameraAngle = 30.0f;

// Protótipos
void initGL();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void desenhaSegmento(const SegmentoRobo &seg);
void desenhaGarra(const Garra &g);
void imprimetTexto(char *str, float x, float y);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bracoo Robtico Articulado");
    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Câmera
    gluLookAt(3,3,3, 0,0,0, 0,1,0);

    // Mostrar instruções
    char info[128];
    sprintf(info, "Segmento: %d | Use teclas: bBase/aAnte/brBraço/...]" ); // implementar detalhes
    
    // Desenha hierarquia
    glPushMatrix();
        // Base móvel
        glTranslatef(0, baseSeg.altura/2, 0);
        glRotatef(baseSeg.anguloY, 0,1,0);
        desenhaSegmento(baseSeg);

        // Braço
        glTranslatef(0, baseSeg.altura/2 + braco.altura/2, 0);
        glRotatef(braco.anguloZ, 0,0,1);
        desenhaSegmento(braco);

        // Antebraço
        glTranslatef(0, braco.altura/2 + antebraco.altura/2, 0);
        glRotatef(antebraco.anguloZ, 0,0,1);
        desenhaSegmento(antebraco);

        // Garra
        glTranslatef(0, antebraco.altura/2 + garra.largura, 0);
        desenhaGarra(garra);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/h, 1.0, 100);
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case '1': selecionado = 0; break;
        case '2': selecionado = 1; break;
        case '3': selecionado = 2; break;
        case '4': selecionado = 3; break;
        case 'a': // rotaciona positiva no eixo Z
            if(selecionado == 1) braco.anguloZ += 5;
            else if(selecionado==2) antebraco.anguloZ += 5;
            else if(selecionado==3) garra.abertura += 2;
            break;
        case 'd': // rotaciona negativa no eixo Z
            if(selecionado == 1) braco.anguloZ -= 5;
            else if(selecionado==2) antebraco.anguloZ -= 5;
            else if(selecionado==3) garra.abertura -= 2;
            break;
        case 'w': // rotação base
            if(selecionado == 0) baseSeg.anguloY += 5;
            break;
        case 's':
            if(selecionado == 0) baseSeg.anguloY -= 5;
            break;
        case 27: // ESC
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if(key == GLUT_KEY_LEFT) cameraAngle -= 5;
    else if(key == GLUT_KEY_RIGHT) cameraAngle += 5;
    gluLookAt(3*sin(cameraAngle*3.1415/180.0), 3, 3*cos(cameraAngle*3.1415/180.0), 0,0,0, 0,1,0);
    glutPostRedisplay();
}

void desenhaSegmento(const SegmentoRobo &seg) {
    glColor3fv(seg.cor);
    glPushMatrix();
        glScalef(seg.largura, seg.comprimento, seg.altura);
        glutSolidCube(1.0);
    glPopMatrix();
}

void desenhaGarra(const Garra &g) {
    glColor3fv(g.cor);
    glPushMatrix();
        // Lado esquerdo
        glPushMatrix();
            glTranslatef(-g.largura/2, 0, 0);
            glRotatef(g.abertura, 0,0,1);
            glTranslatef(0, g.comprimento/2, 0);
            glScalef(g.largura, g.comprimento, g.largura);
            glutSolidCube(1);
        glPopMatrix();
        // Lado direito
        glPushMatrix();
            glTranslatef(g.largura/2, 0, 0);
            glRotatef(-g.abertura, 0,0,1);
            glTranslatef(0, g.comprimento/2,0);
            glScalef(g.largura, g.comprimento, g.largura);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}

void imprimetTexto(char *str, float x, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 0, 600, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glRasterPos2f(x, y);
            for(char* c = str; *c; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}