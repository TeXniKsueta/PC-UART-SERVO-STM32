# PC-UART-SERVO-STM32
 The angle is transmitted from the computer, and the servo rotates to this angle (STM32F103, Servo SG90,USB-UART converter)

![image](https://github.com/TeXniKsueta/PC-UART-SERVO-STM32/assets/152018745/9efbabc5-81fe-4433-8296-6a7c355f91a1)

In this project, the servo control is implemented using Terminal, I send a number to the terminal and the servo is rotated to this number.

![image](https://github.com/TeXniKsueta/PC-UART-SERVO-STM32/assets/152018745/c97d38cd-26c5-412c-835c-b834fc2eaf3f)

To complete this task, you need to turn on TIM2 and turn on the first channel as PWM Generation CH1, 
then you need to change the Prescaler and write the value 22-1 or 21 there,
then you need to write the number 65450 to the CounterPeriod in order to get a frequency equal to 50 Hz (read the datasheet servo sg90).
(do not forget to set the Clock frequency to 72 Mhz in the Clock Configuration)
![image](https://github.com/TeXniKsueta/PC-UART-SERVO-STM32/assets/152018745/0f5507f9-28ff-4ef9-8aed-cde7e695e2b7)

 
 ![image](https://github.com/TeXniKsueta/PC-UART-SERVO-STM32/assets/152018745/97e75829-b1b1-436f-b995-e400293c2f4f)


It is also necessary to enable USART1 in order to transfer information from the Terminal via UART


![image](https://github.com/TeXniKsueta/PC-UART-SERVO-STM32/assets/152018745/9f7e1523-2fc6-417b-98a0-3f9e96b4fb89)


after you do this, copy the code from main.c and stm32f1xx_it.c start the Terminal, make sure to tick +CR 
(in order for the condition that the number was sent and accepted via UART to be fulfilled) 
connect everything as in the screenshot in CubeMX the very first photo and everything will work for you!

Components used in the project:

USB-UART Converter: https://aliexpress.ru/item/1005001621818602.html?sku_id=12000016846616799&spm=a2g2w.productlist.search_results.0.187468d28Y6AAc

SERVO SG90: https://aliexpress.ru/item/32948830521.html?sku_id=66236366285&spm=a2g2w.productlist.search_results.0.75ee3a16LYxocu

STM32F103: https://aliexpress.ru/item/1005006844161938.html?sku_id=12000038490475937&spm=a2g2w.productlist.search_results.13.590294639K0SPK


Information sources used:

STM32 SERVO: https://www.youtube.com/watch?v=qiOir58Yiwc

work with uart: https://mypractic.ru/urok-21-rabota-s-uart-cherez-biblioteku-hal-inicializaciya-interfejsa-i-peredacha-dannyx-v-blokiruyushhem-rezhime-otladka-programm-s-pomoshhyu-uart-funkciya-sprintf.html
