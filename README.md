# Monitor de Temperatura e Tensão

- Equipe envolvida: Low-Battery

- Integrantes: Guilherme Scoz, Murilo Chiquetti Galina e Gabriel Carvalho

- Instituição de ensino: Senai CTAI Florianópolis - Curso técnico em desenvolvimento de sistemas, 3ª Fase

# Situação de Aprendizagem

# 1- Objetivo

    A partir do dispositivo criado em sala utilizando o arduino e nuvem como transmissor e armazenamento destes dados,
    criamos um produto que monitora o seu veiculo em tempo real, envia estes dados ao nosso servidor e ao dashboard no tago.io e verifica se está tudo ok.

# 2- Execução parte lógica do sistema

         "Temperatura de testes"
    SE (A temperatura exceda 23°) { 
        O dispositivo envia um sinal imediato ao usuário ativando o led vermelho, o buzzer e a ventoinha para um breve resfriamento informando um problema que precisa ser resolvido           além disso envia os dados da análise ao nosso dashboard através do server MQTT. Que futuramente será um app disponivel para o usuário também para uma checagem mais rápida. 
    }       
                    "Tensão de testes"
    SE (A tensão da bateria estiver abaixo de 0.4v) {
        O dispositivo envia o mesmo sinal porém ativando o led amarelo e não ativando a ventoinha.
    }

    E SE (Tensão for menor que 0.4 && Temp for maior que 23°) {
        Ativa os dois leds a ventoinha, o buzzer e envia os dados para análise.
    }

    SE NÃO {
        Os leds, buzzer e ventoinha ficam desligados mas continuam enviando os dados ao dashboard.
    }
 
# 3- Conexão com o servidor

    A conexão é feita com o servidor via Client do próprio MQTT utilizando a rede wifi disponibilizada pelo Senai para este projeto, 
    as requisições são realizadas através do nosso Broker com ip padrão do tago.io, e a partir do Token gerado realizamos a conexão.
