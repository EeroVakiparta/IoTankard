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


# TODO

- Should the motor drive 2 directions
- Should I add capasitor for motor
- Come up with the connection to laptop
- Try accelerometer SPI communication with 3 wires instead of 4
- Fix double tap provoking a tap after

## Buttons

MX cherry copies. The key caps are yet to be decided on.

## Motor

6V basic motor was used. Went for low durability solution and soldered a big chunk at the tip of axis.

## Casing

1. "The Wand"

2. "The Remote"

3. "The Tankard