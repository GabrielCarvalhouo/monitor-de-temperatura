# monitor-de-temperatura

Seguindo o exemplo do projeto online criado por nossa equipe usando a ferramenta tinkercad adaptamos o sistema a placa ESP8266 esse protótipo é capaz de ler a temperatura com sensor (LM35), dados que serão enviados a placa em formato de tensão e convertidos em temperatura a partir de um cálculo interno estes dados são comparados dentro de um (if else), se a temperatura estiver acima de 32⁰ a placa manda um sinal digital ao buzzer e aos dois leds para avisar o usuário de que tem algo errado, se caso a temperatura esteja muito baixa ele liga apenas o led vermelho e também alerta o user com o buzzer se não ele deixa os dois leds ligados e não envia nenhum sinal sonoro.