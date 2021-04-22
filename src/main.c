#include <stddef.h>
#include "main.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "string.h"

#define Nsmall 1e-14

unsigned basicOp(CplxNum* x, CplxNum y, const char* cmd, const char* outputmode) {
    

}

unsigned complexsub(CplxNum* c1, CplxNum c2) {
   

}

unsigned complexdvs(CplxNum* c1, CplxNum c2) {
   

}

unsigned polarinvertion(Pol* w, Pol p) { 
    // Normaliza o �ngulo de p
    //  A norma de p ser� sempre positiva e 
    //  o �ngulo ser� sempre entre -M_PI < p.g <= M_PI
    normalizeangle(&p);
    // Se a norma � zero, ent�o o angulo tamb�m � zero
    if (fabs(p.r - 0.0) < Nsmall)
        p.g = 0;
    // N�o modifique o c�digo acima
    // Implemente seu c�digo abaixo


}

unsigned negativecartesian(Cart* x, Cart y) {
   

}

unsigned somacartesiana(Cart* x, Cart y) {

    x->a = x->a + y.a;
    x->b = x->b + y.b;

    return 0;
}

unsigned cartesian2polar(Pol* w, Cart x) {

    w->r = sqrt(x.a * x.a + x.b * x.b);
    w->g = atan2(x.b, x.a);

    return 0;
}

unsigned multiplypolar(Pol* w, Pol p) {

    // Testa para uma ou outra entrada zero
    if (!(w->r != 0.0) || !(p.r != 0)) {
        w->g = 0.0;
        w->r = 0.0;
        return 0;
    }

    // Soma os �ngulos
    w->g = w->g + p.g;
    normalizeangle(w);

    // Testa para uma ou outra entrada infinita
    if (w->r >= INFINITY || p.r >= INFINITY) {
        w->r = INFINITY;
        return 2;
    }

    // Testa se as duas entradas tem n�meros gigantescos
    if (w->r >= _HUGE_ENUF && p.r >= _HUGE_ENUF) {
        w->r = INFINITY;
        return 2;
    }

    // Multiplica as normas
    w->r = w->r * p.r;

    return 0;
}

// Normaliza o �ngulo no intervalo PI > �ngulo >= PI
unsigned normalizeangle(Pol* w) {
    long double angle, n_angle, frac;
    long long int integ;

    n_angle = w->g / (2 * M_PI); // Quantidade de 2PI existem em w->g
    integ = (long long int) n_angle; // Quantidades inteiras de 2PI existem em n_angle
    frac = (n_angle - (long double)integ); // Separa a fra��o de 2PI existe em n_angle
    angle = frac * 2 * M_PI; // 0 <= |angle| < 2*PI
    if (angle >= 0) {
        if (angle > M_PI) {
            w->g = -(2 * M_PI - angle);
        }
        else {
            w->g = angle;
        }
    }
    else {
        angle = -angle;
        if (angle > M_PI) {
            w->g = (2 * M_PI - angle);
        }
        else {
            w->g = -angle;
        }
    }
    // Marretada num�rica
    // Se o �ngulo estiver na vizinhan�a de PI, ent�o � PI
    if ((M_PI - fabsl(w->g)) < Nsmall) {
        w->g = M_PI;
    }
    
    // Se a norma de w for menor que zero,
    //  subtrai PI (180o) do angulo de w
    //  inverte o sinal da norma de w
    //  renormaliza
    if (w->r < 0) {
        w->g = w->g - M_PI;
        w->r = -w->r;

        // Renormaliza
        angle = w->g;
        if (angle >= 0) {
            if (angle > M_PI) {
                w->g = -(2 * M_PI - angle);
            }
            else {
                w->g = angle;
            }
        }
        else {
            angle = -angle;
            if (angle > M_PI) {
                w->g = (2 * M_PI - angle);
            }
            else {
                w->g = -angle;
            }
        }
        // Marretada num�rica
        // Se o �ngulo estiver na vizinhan�a de PI, ent�o � PI
        if ((M_PI - fabsl(w->g)) < Nsmall) {
            w->g = M_PI;
        }
    }

    return 0;
}

unsigned polar2cartesian(Cart* x, Pol w) {

    if (w.r == 0.0) {
        x->a = 0.0;
        x->b = 0.0;
        return 0;
    }

    if (w.r >= INFINITY) {
        x->a = INFINITY;
        x->b = INFINITY;
        return 2;
    }

    if (w.g >= INFINITY) {
        x->a = INFINITY;
        x->b = INFINITY;
        return 2;
    }

    // normalizeangle(&w);

    x->a = w.r * cos(w.g);
    x->b = w.r * sin(w.g);

    return 0;
}

unsigned complexsum(CplxNum* c1, CplxNum c2) {

    Cart c1_cart, c2_cart;
    unsigned ret;

    if (strcmp(c1->mode, cartesian) == 0) {
        if (strcmp(c2.mode, polar) == 0) {
            // Transforma c2 para cartesiano em c2_cart
            ret = polar2cartesian(&c2_cart, c2.s.pol);
            if (ret != 0) {
                return ret;
            }
            // Soma c1 e c2_cart
            ret = somacartesiana(&c1->s.cart, c2_cart);
            if (ret != 0) {
                return ret;
            }
        }
        else {
            // Soma c1 e c2
            ret = somacartesiana(&c1->s.cart, c2.s.cart);
            if (ret != 0) {
                return ret;
            }
        }
    }
    else {
        if (strcmp(c2.mode, polar) == 0) {
            // transforma c1 e c2 para cartesiano em c1_cart e em c2_cart
            ret = polar2cartesian(&c1_cart, c1->s.pol);
            if (ret != 0) {
                return ret;
            }
            ret = polar2cartesian(&c2_cart, c2.s.pol);
            if (ret != 0) {
                return ret;
            }
            // soma c1_cart e c2_cart
            ret = somacartesiana(&c1_cart, c2_cart);
            if (ret != 0) {
                return ret;
            }
            // transforma c1_cart para polar em c1
            ret = cartesian2polar(&c1->s.pol, c1_cart);
            if (ret != 0) {
                return ret;
            }
        }
        else {
            // transforma c1 para cartesiano em c1_cart
            ret = polar2cartesian(&c1_cart, c1->s.pol);
            if (ret != 0) {
                return ret;
            }
            // soma c1_cart e c2
            ret = somacartesiana(&c1_cart, c2.s.cart);
            if (ret != 0) {
                return ret;
            }
            // transforma c1_cart para polar em c1
            ret = cartesian2polar(&c1->s.pol, c1_cart);
            if (ret != 0) {
                return ret;
            }
        }
    }

    return 0;
}

unsigned complexmultiply(CplxNum* c1, CplxNum c2) {
    Pol c1_pol, c2_pol;
    unsigned ret;

    if (strcmp(c1->mode, cartesian) == 0) {
        if (strcmp(c2.mode, polar) == 0) {
            // transforma c1 para polar em c1_pol
            ret = cartesian2polar(&c1_pol, c1->s.cart);
            if (ret != 0) {
                return 2;
            }
            // multiplica c1_pol por c2
            ret = multiplypolar(&c1_pol, c2.s.pol);
            if (ret != 0) {
                return 2;
            }
            // transforma c1_pol para cartesiano em c1
            ret = polar2cartesian(&c1->s.cart, c1_pol);
            if (ret != 0) {
                return 2;
            }
        }
        else {
            // transforma c1 para polar em c1_pol
            ret = cartesian2polar(&c1_pol, c1->s.cart);
            if (ret != 0) {
                return 2;
            }
            // transforma c2 para polar em c2_pol
            ret = cartesian2polar(&c2_pol, c2.s.cart);
            if (ret != 0) {
                return 2;
            }
            // multiplica c1_pol por c2_pol
            ret = multiplypolar(&c1_pol, c2_pol);
            if (ret != 0) {
                return 2;
            }
            // transforma c1_pol para cartesiano em c1
            ret = polar2cartesian(&c1->s.cart, c1_pol);
            if (ret != 0) {
                return 2;
            }
        }
    }
    else {
        if (strcmp(c2.mode, polar) == 0) {
            // multiplica c1 por c2
            ret = multiplypolar(&c1->s.pol, c2.s.pol);
            if (ret != 0) {
                return 2;
            }
        }
        else {
            // transforma c2 para polar em c2_pol
            ret = cartesian2polar(&c2_pol, c2.s.cart);
            if (ret != 0) {
                return 2;
            }
            // multiplica c1 por c2_pol
            ret = multiplypolar(&c1->s.pol, c2_pol);
            if (ret != 0) {
                return 2;
            }
        }
    }

    return 0;
}

// Descri��o do Assignment

/*
Este assignment � uma continua��o do anterior e cont�m todas 
as suas fun��es implementadas.

Como sabemos, a soma e a subtra��o de n�meros complexos � mais
f�cil e direta quando os operandos est�o na base cartesiana.
Analogamente, � prefer�vel realizar a multiplica��o e a 
divis�o na base polar.
Assim, este projeto utiliza desta estrat�gia, transformando a 
base dos operandos quando necess�rio para realizar as opera��es, 
mas transformando o resultado na base desejada.


Sua tarefa � construir a fun��o basicOp e fazer com que seu c�digo
passe nos testes unit�rios especificados neste projeto do VS.
Al�m disso, voc� deve implementar as fun��es polarinversion,  
negativecartesian, complexsub e complexdvs, necess�rias para 
realizar as opera��es de divis�o e subtra��o de complexos.


A fun��o negativecartesian deve produzir o (-1) de um n�mero complexo
na base cartesiana. Sua entrada � Cart y e a sa�da � Cart *x. A 
fun��o retorna zero se tudo correu bem.
Esta fun��o permite realizar a subtra��o, utilizando-a em conjunto com
a fun��o complexsum. A subtra��o nada mais � que a soma de dois 
n�meros com o segundo n�mero multiplicado por (-1).
A fun��o sempre retorna zero.


A fun��o polarinversion produz o (1/n) de um n�mero complexo n na
base polar. Sua entrada � Polar p e a sa�da � Polar* w.
Esta fun��o permite que seja realizada a divis�o, usando-a em conjunto
com a fun��o complexmultiply. A divis�o nada mais � que a 
multiplica��o de dois n�meros, com o segundo n�mero invertido (1/n).

A fun��o, antes de realizar qualquer opera��o, normaliza o �ngulo 
do n�mero complexo de entrada verifica se o m�dulo deste � zero. Se
for, zera tamb�m seu �ngulo.
N�o existe �ngulo para um vetor com norma zero! Ent�o, por defini��o,
o �ngulo deste vetor � zero.
Esta parte do c�digo j� � fornecida.

A fun��o polarinversion retorna zero se tudo correu bem. Ela retorna 2 
caso o m�dulo do n�mero entrado seja menor ou igual a 1/_HUGE_ENUF. 
Neste caso, a fun��o devolde INFINITY como m�dulo de sua vari�vel de 
sa�da.
A fun��o polarinversion tamb�m retorna zero se o m�dulo da entrada 
for maior ou igual a _HUGE_ENUF. Neste caso, o m�dulo da sa�da ser�
zero e, por consequ�ncia, seu �ngulo tamb�m ser� zero.


A fun��o normalizeangle foi modificada. Agora, se o n�mero entrado
tiver norma negativa, a sa�da ter� norma positiva com o �ngulo
subtra�do de PI radianos. O �ngulo de sa�da estar�, como na 
vers�o anterior, entre -PI < ang <= PI.
O c�digo com estas modifica��es est� dado.


A fun��o complexsub realiza a subtra��o de dois n�meros, quaisquer
que sejam as suas bases. Esta fun��o deve usar as fun��es complexsum, 
negativecartesian e quaisquer outras necess�rias para implementar a 
subtra��o.
A fun��o complexsub realiza a seguinte opera��o com seus operandos:
c1 = c1 - c2. O operando c1 tamb�m � o operando de sa�da e a base do
resultado ser� a base deste operando.
Esta fun��o retorna zero se a opera��o for bem sucedida e retorna 2
caso ocorram problemas num�ricos.


A fun��o complexdvs realiza a divis�o de dois n�meros complexos, 
quaisquer que sejam suas bases. Esta fun��o deve usar as fun��es 
complexmultiply, polarinversion e quaisquer outras que sejam
necess�rias para implementar a divis�o.
A fun��o complexdvs realiza a seguinte opera��o com seus operandos:
c1 = c1/c2. O operando c1 tamb�m � o operando de sa�da e a base do
resultado ser� a mesma deste operando.
Esta fun��o retorna zero se a opera��o for bem sucedida e retorna 2
caso ocorram problemas num�ricos.


A fun��o basicOp realiza as quatro opera��es b�sica sobre seus
operandos x e y, de forma que x = x op y, onde op pode ser uma
opera��o de soma, subtra��o, multiplica��o ou divis�o.
Os operandos x e y s�o n�meros complexos cujas partes real e
imagin�ria s�o representadas em ponto flutuante com dupla precis�o
(double). Estes operandos podem ser representados tanto em
coordenadas cartesianas (a + bj) quanto e coordenadas polares
(r*(exp(jg)), onde a e b representam as por��es real e imagin�ria
e r e g representam o m�dulo e o �ngulo do vetor no plano s
(imagin�rio).
Os campos ClpxNum *x e y s�o as vari�veis passadas para a fun��o.
O campo cmd define qual opera��o ser� realizada por basicOp. A
fun��o aceita apenas as strings: "add", "sub", "mul" e "dvs" para
realizar a opera��o de soma, subtra��o, multiplica��o ou divis�o 
sobre os operandos, respectivamente.
O campo outputmode define em qual sistema de coordenadas ser�
escrito o resultado da opera��o, desta forma pode apenas receber o
endere�o de uma das duas strings "cart" ou "pol" ou NULL. Neste
�ltimo a fun��o mant�m a base original do operador de sa�da, sem
necessidade de realizar uma convers�o de base ao final.
A fun��o retorna zero se a opera��o for bem sucedida, retorna 1
caso algum operando ou campo da fun��o aponte para strings n�o
definidas e retorna 2 caso ocorra uma divis�o por zero ou outro
erro num�rico.
Para implementar as fun��es voc� DEVE utilizar as fun��es b�sicas 
fornecidas e implementadas, sempre que for poss�vel. Na avalia��o do 
seu programa isso tem grande peso.

*/


// Descri��o do Assignment anterior (SEM10)

// Neste assigment voc� deve continuar o desenvolvimento das
// fun��es que comp�e uma pequena biblioteca que implementa
// as opera��es aritm�ticas b�sicas para n�meros complexos
// na base cartesiana ou na base polar.
// 
// Nesta nova parte do projeto, utilizaremos as fun��es que
// voc� implementou no assignment da Semana 9.
// Assim, voc� deve copiar o c�digo constru�do para as fun��es 
// do projeto da Semana 9 e colar neste novo projeto
//
// As especifica��es das fun��es implementadas no Assignment
// semana 9 continuam, em grande parte, as mesmas e seguem logo
// abaixo desta nova descri��o.
// No entanto, as fun��es normalizeangle e multiplypolar 
// tiveram seus requisitos alterados de forma a melhorar seu 
// desempenho e para atender os requisitos das novas fun��es
// que ser�o implementadas.
// Sendo assim, testes adicionais para estas fun��es foram 
// implementados.
//
// A fun��o normalizeangle recebe como par�metro de entrada 
// um tipo Pol* cujo argumento do �ngulo pode ter qualquer 
// amplitude em radianos, incluindo valores positivos e 
// negativos.
// A fun��o agora normaliza o valor deste argumento de forma 
// que este esteja entre -PI < �ngulo <= PI. Note que o 
// intervalo foi ligeiramente modificado.
// Outra modifica��o fornecida, que deve ser incorporada ao 
// final do seu c�digo na fun��o, trata da descontinuidade 
// e da imprecis�o ou erros inerentes aos valores em 
// long double em torno do PI radianos.
// Imagine um �ngulo calculado cujo valor deveria ser exatos 
// PI radianos. Este �ngulo raramente ter� este valor, devido 
// �s aproxima��es do processamento em ponto flutuante. Na 
// realidade, este �ngulo ir� oscilar com um erro E entre 
// -PI+E e PI-E. Onde E � o erro de aproxima��o inserido pelas
// opera��es em ponto flutuante.
// Isso produz resultados inesperados e em grande parte
// errados quando o �ngulo do n�mero normalizado estiver
// nesta regi�o.
// Assim, sempre que o valor calculado estiver dentro desta
// regi�o de erro, o c�digo ir� aproximar automaticamente
// o resultado para PI, conforme pode ser visto no trecho 
// de c�digo disponibilizado.
//
// A fun��o cartesian2polar agora deve retornar valores de
// �ngulo no intervalo entre -PI < �ngulo <= PI.
//
// A nova vers�o da fun��o multiplypolar deve retornar o 
// valor do �ngulo calculado normalizado entre 
// -PI < �ngulo <= PI. Para isso, voc� deve obrigatoriamente
// usar a fun��o normalizeangle, chamando-a dentro da
// fun��o multiplypolar.
//
// A fun��o polar2cartesian recebe um valor Pol w em 
// coordenadas polares e o converte para cartesiano, 
// retornando-o na vari�vel do tipo Cart apontada por x.
// Esta fun��o deve verificar se o valor do m�dulo do
// n�mero complexo de entrada � zero, ent�o deve retornar
// um vetor com coordenadas cartesianas zero.
// Adicionalmente a fun��o testa se os valores do m�dulo ou 
// do �ngulo do n�mero entrado s�o INFINITY. Caso sejam, a
// fun��o deve retornar um par de coordenadas INFINITY.
//
// Para implementar as fun��es complexsum e complexmultiply 
// � preciso entender como � aplicado o tipo ComplexNum, o  
// qual � definido em main.h
// ComplexNum � definido por uma struct que cont�m dois 
// campos, um do tipo Clpx e outro do tipo char*. O primeiro 
// cont�m um valor complexo s, que pode conter em um certo
// momento um valor na base cartesiana ou um valor na base
// polar. O segundo (mode) � um ponteiro para a string cujo 
// conte�do define qual a base do n�mero complexo armazenado 
// em s.
// Nas linhas 5 e 6 de main.h est�o definidas as strings 
// polar e cartesian. Estas s�o as strings para qual o 
// campo mode de uma vari�vel do tipo ClpxNum deve apontar,
// dependendo, como j� dito, da base do n�mero complexo
// armazenado no campo s desta mesma vari�vel.
// O campo s � do tipo Clpx, que � uma Union das structs 
// Cart e Pol. Isto permite que o campo s possa conter valores
// do tipo Cart ou do tipo Pol, ou seja, s pode representar
// tanto um n�mero complexo na base cartesiana quanto um
// n�mero complexo na base polar.
// Por exemplo, se uma vari�vel do tipo ClpxNum contiver
// um n�mero complexo cartesiano, ent�o, seu campo mode ir� 
// apontar para a string cartesian. Desta forma, o campo s
// conter� valores das coordenadas cartesianas do n�mero.
// Da mesma forma, se o campo s contiver um n�mero complexo 
// polar, o campo mode ir� apontar para a string polar. 
// Consequentemente o campo s ter� conter� sub-campos com as
// coordenadas polares do n�mero.
// 
// A fun��o complexsum soma dois n�meros complexos c1 e c2, 
// encapsulados com o tipo ClpxNum.
// Nossa biblioteca conseque apenas somar n�meros na base
// cartesiana. A biblioteca tamb�m possui fun��es para 
// converter n�meros da base cartesiana para a base polar e
// vice-versa.
// A fun��o complexsum pode somar dois n�meros complexos, os 
// quais podem estar em qualquer base. A fun��o realiza a 
// seguinte opera��o: c1 = c1 + c2.
// Se necess�rio, a fun��o converte uma c�pia dos n�meros
// a serem somados para a base cartesiana, soma os mesmos,
// converte o resultado para a base original de c1 e 
// retorna o resultado para c1.
// A fun��o retorna zero se n�o houver erros. Eventualmente
// caso alguma fun��o de convers�o retorne um c�digo de erro
// (deferente de zero), ent�o a fun��o complexsum p�ra sua
// execu��o e retorna o erro.
//
// A fun��o complexmultiply opera de an�loga � fun��o 
// complexsum. Nossa biblioteca somente consegue multiplicar
// n�meros na base polar.
//
// Para a implementa��o das fun��es deste assignment �
// mandat�rio que as fun��es b�sicas constru�das sejam
// utilizadas dentro das fun��es de mais alto n�vel
// quando necess�rio. O uso de c�digo redundante ir� 
// tornar seu exerc�cio errado, mesmo que esteja funcional.
//


// Descri��o do Assignment anterior (SEM9)

/*
Sua tarefa � �mplementar as fun��es acima prototipadas
Estas fun��es usam as structs, unions e typedefs definidos em
main.h. Estas defini��es permitem a manipula��o de n�meros
complexos no sistema de coordenadas cartesiano (a+bj) ou no
sistema polar (r*exp(jg)) (r=m�dulo, g=�ngulo).

O objetivo � criar fun��es b�sicas para serem utilizadas na
constru��o de fun��es mais elaboradas, que permitir�o lidar
com os problemas inerentes aos n�meros complexos de maneira
transparente ao usu�rio.

Estas fun��es b�sicas, apesar de implementarem opera��es
matem�ticas elementares, lidam com alguns problemas num�ricos
importantes. Assim, os testes criados em test.cpp verificam
a implementa��o de cada uma destas quatro fun��es iniciais.
Os testes est�o agrupados em t�picos referentes a cada uma
das fun��es a serem testadas, como pode ser visto no Test
Explorer.

A fun��o somacartesiana recebe dois par�metros do tipo Cart
e realiza a seguinte opera��o vetorial sobre estes:
x = x + y,
onde dois n�meros complexos (x, y) que est�o na forma
cartesiana, s�o somados.
Nesta vers�o, esta fun��o sempre deve retornar zero.

A fun��o cartesian2polar converte um n�mero na forma
cartesiana (x) em n�mero na forma polar (w), onde x e w
s�o os par�metros de entrada e de sa�da da fun��o,
respectivamente.
Esta fun��o converte vetores cartesianos nos quatro
quadrantes. A fun��o utiliza como unidade angular o radiano.
Na sa�da, o �ngulo (g) estar� entre -PI <= g < PI (rad).
A implementa��o de algumas das fun��es trigonom�tricas
tradicionais em math.h possuem problemas em diferentes
quadrantes e nos seus limites. Sua tarefa � pesquisar e
encontrar a solu��o.
Nesta vers�o, esta fun��o sempre deve retornar zero.

A fun��o multiplypolar realiza a seguinte opera��o sobre
n�meros na forma polar:
w = w * p
Como a fun��o poder� ser utilizada para a divis�o de dois
n�meros, esta testa para alguns problemas n�mericos comums.
Caso o m�dulo de qualquer um dos operadores for zero, ent�o
o resultado retornado ser� o vetor polar (0,0), pois n�o h�
defini��o de �ngulo para vetor com comprimento zero.
Nas bibliotecas utilizadas existe a constante INFINITY, que
define um n�mero o qual valores acima deste s�o considerados
n�meros infinitos. Assim, caso algum dos m�dulos dos par�metros
fornecidos para a fun��o forem iguais ou maiores que INFINITY,
ent�o a sa�da ter� m�dulo INFINITY. O �ngulo n�o � afetado.
Nas bibliotecas tamb�m � definido a constante _HUGE_ENUF. Caso
ambos os par�metros de entrada forem maiores que _HUGE_ENUF,
ent�o a fun��o deve retornar INFINITY. O �ngulo n�o � afetado.
Verifique como � definida a constante INFINITY e entenda o
porqu� desta regra.
A fun��o retorna zero normalmente e retorna 2 caso o m�dulo do
resultado retornado seja INFINITY.

A fun��o normalizeangle normaliza o �ngulo de um n�mero
complexo (w) na forma polar. O n�mero entrado pode ter um �ngulo
qualquer expresso em radianos, inclusive com valor negativo.
A fun��o deve converter o �ngulo entrado para o intervalo
positivo entre -PI <= g < PI (rad).
A norma do vetor n�o � processada, permanecendo inalterada.
Esta fun��o sempre deve retornar zero.

*/
