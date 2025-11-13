# Objetos-inteligentes-conectados

# Descrição do Projeto

Esse projeto tem o objetivo de criar um eletrocardiograma caseiro utilizando componentes de fácil acesso e baixo custo.

# Componentes

Placa ESP32 DevKit V1
Módulo AD8232 com sensores (eletrodos)
Buzzer passivo
Protoboard
Jumpers fêmea-fêmea e macho-fêmea

# Funcionamento

A placa ESP32 faz a leitura do sinal analógico, gerado pelos sensores do módulo AD8232, que capta o ritmo cardíaco do usuário. O ESP32 processa os valores, calcula o BPM (batimentos por minuto) e envia o resultado ao broker público HiveMQ por meio do protocolo MQTT. Além disso, um buzzer passivo é acionado a cada detecção de batimento, funcionando como alerta sonoro de confirmação. Os valores de BPM podem ser monitorados em tempo real no software MQTT Explorer ou em qualquer cliente MQTT compatível.

# Código-fonte

Disponível no arquivo "main.ino".

# Diagrama de montagem

Disponível no arquivo "fritzing.fzz"

# Conexão MQTT Explorer

Disponível no arquivo "mqtt_explorer.png".

# Configurações do protocolo MQTT

Broker: broker.hivemq.com

Porta: 1883

Tópico (publish): maeda/esp32/bpm

Biblioteca utilizada: PubSubClient

O ESP32 atua exclusivamente como publisher, enviando os valores de BPM calculados para o broker MQTT.
A comunicação ocorre via TCP/IP, sobre a rede Wi-Fi configurada no código.

# Resultado esperado

Exemplo de saída no monitor serial:
WiFi conectado!
Broker conectado!
BPM atual: 60.0
BPM atual: 62.5

Exemplo de saída no MQTT Explorer em tempo real:
BPM=62.5