# IoTankard

Curious IoT test project.


# Pin allocation

| Device        | Pins              | Notes                  |
| ------------- |:-------------:    | ----------------------:|
| MX Buttons    | D4,D5,D6,D7       |  Add 10k resistors     |
| HC-05 BT      | D2,D3             |   RX, TX               |
| OLED I2D      | A4,A5             |   SCL, SDA             |
| ADXL345       | D10,D11,D12,D13   |        SPI             |
| MotorDriver   | D9 (PWM)          |    Only one needed     |
| IF            | A6                |                        |

3 free digital and 5 analog pins