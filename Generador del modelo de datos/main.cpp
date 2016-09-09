#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/**
 * Devuelve en un array las coordenas 'x' e 'y' con el angulo respecto al origen de coordenadas (0, 0) y su distancia al mismo...
 */
void getXY(double angulo, double distancia, double* p)
{
    p[0] = distancia * cosl(angulo);
    p[1] = distancia * sinl(angulo);
}

/**
 * Devuelve la distancia entre dos coordenadas...
 */
double getDistancia(double x1, double y1, double x2, double y2)
{
    //lo resuelvo por el teorema de pitagoras...
    double c1 = x2 - x1;
    double c2 = y2 - y1;

    double res = sqrtl(powl(c1, 2) + powl(c2, 2));
    return res;
}

/**
 * Determina si los 3 puntos dados están alineados...
 */
bool estanAlineados(double x1, double y1, double x2, double y2, double x3, double y3)
{
    if (x1 == x2)
    {
        if (x2 == x3) //Si las 3 'x' son iguales, es una recta paralela al eje 'y'
            return true;
        return false; //Porque si solo 2 'x' son iguales, forman una recta paralela al eje 'y' dejando al punto que tiene la tercer 'x' fuera de esta recta...
    }
    else if (x2 == x3)
        return false; //Porque si solo 2 'x' son iguales, forman una recta paralela al eje 'y' dejando al punto que tiene la tercer 'x' fuera de esta recta...

    if (y1 == y2)
    {
        if (y2 == y3) //Si las 3 'y' son iguales, es una recta paralela al eje 'x'
            return true;
        return false; //Porque si solo 2 'y' son iguales, forman una recta paralela al eje 'x' dejando al punto que tiene la tercer 'y' fuera de esta recta...
    }
    else if (y2 == y3)
        return false; //Porque si solo 2 'y' son iguales, forman una recta paralela al eje 'x' dejando al punto que tiene la tercer 'y' fuera de esta recta...

    //Obtengo la funcion de la recta que pasa por ($x1, $y1) y ($x2, $y2): y = $mx + $c
    double m = (y2 - y1) / (x2 - x1);
    double c = (m * (-1.0 * x1)) + y1;

    //Compruebo si el tercer punto ($x3, $y3) pertenece a la recta...
    double res = (m * x3) + c;
    if (res == y3)
        return true;
    else
        return false;
}

/**
 * Devuelve el area del triangulo formado por las 3 coordenadas...
 */
double getAreaTriangulo(double x1, double y1, double x2, double y2, double x3, double y3)
{
    //Es un método obtenido de internet...
    double area = ((x1 * (y3 - y2)) + (x3 * (y2 - y1)) + (x2 * (y1 - y3))) / 2.0;
    if (area < 0.0)
        area *= -1.0;

    return area;
}

/**
 * Funcion para facilitar la suma (o resta) de grados...
 */
double sumaGrados(double grados, double cant_grados_a_sumar)
{
    double res = grados + cant_grados_a_sumar;
    if (res > 360.0)
        res -= 360.0;
    if (res < 0.0)
        res += 360.0;

    return res;
}

/**
 * Funcion que inserta un registro en la DB...
 */
void insertarPrediccion(int dia, double gV, double dV, double gF,
    double dF, double gB, double dB, int hay_sequia, int hay_lluvia,
    int hay_condic_optima, double perimetro, ofstream& fs)
{
    int size = 1024*4;
    char buff[size];

    snprintf(buff, size, " insert into condicion_metereologica (dia, gv, dv, gf, df, gb, db, "
            "        hay_sequia, hay_lluvia, hay_condic_optima, perimetro) "
            " values "
            "        (%d, %f, %f, %f, %f, %f, %f, %d, "
            "         %d, %d, %f); \n", dia, gV, dV, gF, dF, gB, dB, hay_sequia,
            hay_lluvia, hay_condic_optima, perimetro);

    fs.write(buff, strlen(buff));
}

/**
 * Funcion que predice las condiciones climaticas de los $dias dias...
 */
void predecir(int dias, double vV, double vF, double vB,
    double dV, double dF, double dB, ofstream& fs)
{
    //Posicion en grados de los planetas... Por defecto inician en 0 todos...
    double gV = 0.0;
    double gF = 0.0;
    double gB = 0.0;

    //Posicion en coordenadas de los planetas...
    double pV[2];
    double pF[2];
    double pB[2];

    //Areas de los triangulos...
    double aVFS = 0.0;
    double aVBS = 0.0;
    double aFBS = 0.0;
    double aVFB = 0.0;

    //Datos para la DB...
    int hay_sequia        = 0;
    int hay_lluvia        = 0;
    int hay_condic_optima = 0;
    double perimetro      = 0.0;
    for (int x = 0; x < dias; ++x)
    {
        //Obtengo las coordenadas de los planetas...
        getXY(gV, dV, pV);
        getXY(gF, dF, pF);
        getXY(gB, dB, pB);

        //Inicializo los datos para la DB...
        hay_lluvia = 0;
        perimetro = -1.0;
        hay_sequia = 0;
        hay_condic_optima = 0;

        //Me fijo si estan alineados...
        if (estanAlineados(pV[0], pV[1], pF[0], pF[1], pB[0], pB[1]))
        {
            //Me fijo si estan alineados con el sol...
            if (estanAlineados(pV[0], pV[1], pF[0], pF[1], 0.0, 0.0))
                hay_sequia = 1;
            else
                hay_condic_optima = 1;
        }
        else //Los planetas forman un triangulo...
        {
            //Obtengo el perimetro del triangulo...
            perimetro = getDistancia(pV[0], pV[1], pF[0], pF[1]) + getDistancia(pF[0], pF[1], pB[0], pB[1]) +
                getDistancia(pB[0], pB[1], pV[0], pV[1]);

            //Para chequear si el sol está dentro del triangulo que forman los planetas
            //calculo las areas de los triangulos que forman el eje de coordenadas con 2
            //de los 3 puntos, entonces si el eje de coordenadas (sol) esta dentro del
            //triangulo, la suma de las areas de los triangulos que arme con el sol
            //deberia ser igual al area del triangulo formado por los planetas... Si
            //es distinta, el sol no esta dentro del triangulo formado por los planetas...

            //Area del triangulo formado por Vulcano, Ferengis y el Sol...
            aVFS = getAreaTriangulo(pV[0], pV[1], pF[0], pF[1], 0.0, 0.0);
            //Area del triangulo formado por Vulcano, Betasoides y el Sol...
            aVBS = getAreaTriangulo(pV[0], pV[1], pB[0], pB[1], 0.0, 0.0);
            //Area del triangulo formado por Ferengis, Betasoides y el Sol...
            aFBS = getAreaTriangulo(pF[0], pF[1], pB[0], pB[1], 0.0, 0.0);

            //Area del triangulo formado por Vulcano, Ferengis y Betasoides...
            aVFB = getAreaTriangulo(pV[0], pV[1], pF[0], pF[1], pB[0], pB[1]);

            if ((aVFS + aVBS + aFBS) == aVFB)
                hay_lluvia = 1;
            else
                hay_lluvia = 0;
        }

        //Inserto la prediccion en la DB...
        insertarPrediccion((x+1), gV, dV, gF, dF, gB, dB, hay_sequia, hay_lluvia, hay_condic_optima, perimetro, fs);

        //Muevo los planetas...
        gV = sumaGrados(gV, vV);
        gF = sumaGrados(gF, vF);
        gB = sumaGrados(gB, vB);
    }
}


int main(int argc, char *argv[])
{
    //Distancia del sol...
    double dV = 1000.0;
    double dF = 500.0;
    double dB = 2000.0;

    //Velocidad de los planetas (grados * dia)...
    double vV = -5.0;
    double vF = 1.0;
    double vB = 3.0;

    ofstream fs("datos.txt");

    predecir(3600, vV, vF, vB, dV, dF, dB, fs);
    printf("Termino el procesamiento\n");

    return 0;
}
