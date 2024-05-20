//Acendendo lâmpada com sensor de som

int microfone = 8; // Pino digital ligado ao sensor de som (DO)
int rele = 7; // Pino ligado ao rele

int contPalmas = 0; // Contador de Palmas
int palmasLigar = 2; // Contagem para ligar o rele
int palmasDesligar = 2; // Contagem para desligar o rele
// Tempo máximo entre o pulso inicial e o seguinte
unsigned long tempMaxSom = 1000;
unsigned long tempMinSom = 300;
unsigned long compriSonoro = 150; // Comprimento do som esperado
unsigned long time;
unsigned long startTime = 0;

void setup() {
pinMode(microfone, INPUT); // Inicia o pino do microfone como entrada
pinMode(rele, OUTPUT); // Inicia os pino do rele como saída
// Desliga rele (o rele é invertido HIGH = desliga / LOW = liga)
digitalWrite(rele, HIGH);
}

void loop() {
// Inicia a contagem de tempo
time = millis();
// Verifica o tempo entre o pulso inicial e o seguinte
unsigned long tempoAposPalma = time - startTime;

if (tempoAposPalma >= compriSonoro && digitalRead(microfone) == LOW) {

// Verifica se o pulso recebido respeita o intervalo entre 1 pulso e outro
if (tempoAposPalma < tempMinSom || tempoAposPalma > tempMaxSom) {

// Caso contrario o intervalo resetara a contagem e o tempo
contPalmas = 0;
startTime = millis();
}
else {

// Iniciada a contagem de pulso e o tempo
contPalmas ++;
startTime = millis();
}

// Verifica se a contagem de palma é igual ou superior ao número...
//esperado e se o rele esta desligado
if((contPalmas>=palmasLigar-1)&&(digitalRead(rele)==HIGH)){

// Acende o rele e zera a contagem
digitalWrite(rele, LOW);
contPalmas = 0;
}

// Verifica se a contagem de palma é igual ou superior ao número...
//esperado e se o rele esta aceso
if((contPalmas>=palmasDesligar-1)&&(digitalRead(rele)==LOW)){

// Desliga rele e zera contagem
digitalWrite(rele, HIGH);
contPalmas = 0;
}
}
}
