# 微型长电池寿命多功能户外应急灯

## 有啥用：
   无意间看到淘宝上有类似的户外应急灯，功能比较单一（只有一个荧光灯），还卖的不便宜，正好入手了一些超小号的8引脚单片机，放着也没啥用，自己做一个吧
   平时可以挂在钥匙链上，夜间紧急停车的时候，可以开着频闪放在车后当警示，也能当手电用，登山的时候挂包上，开着荧光节能模式，一闪一闪像萤火虫，可以防止后面队友跟丢（或者是自己丢了方便被找到）。

## 功能：
1. 长电池寿命：长亮模式2小时，5秒频闪模式24小时，长效模式（15秒频闪）3天，关机下漏电nA级别，可长期保存
2. 多种功能：
   * 长效荧光灯:用紫外光周期激发高亮荧光粉，从而以极低的功耗获得不错的照明效果，支持常亮、5秒、15秒三种模式
   * 白光手电：常亮、1秒频闪、5秒频闪三种模式
   * 橙色雾灯：常亮、1秒频闪、5秒频闪三种模式
   * 红色警示灯：常亮、1秒频闪、5秒频闪三种模式
   * 防误触开机：长按+4次短按开机，误触会有频闪提示，防止装兜里不小心打开
3. 户外设计：
   * 全密封灌胶防尘防水

## 设计细节：
1. 大小35mm*35mm*30mm，电池使用LR2477纽扣电池，标称容量190mAh，放电电流100mA，一次只能至多点亮一个LED(60mA)，更多的话需要考虑另一颗电池并联
2. 比较经典的TP4056充电+PW5410B电荷泵升降压到3.3V+USB、电池切换的设计，只是加了一个双MOS管的防止电池向TP4056漏电的设计，从而延长了保存时间
3. 8引脚的STC8G17A单片机+TCA9534A扩展IO，很皮实，而且自带低电压检测功能，国产低端芯片很强大
4. 灌注了两次环氧树脂胶+各种打磨，需要手工比较多，量产的话可以考虑3D打印外壳

# Miniature Multifunctional Outdoor Emergency Light with Long Battery Life
##What It's Used For
I happened to notice some similar outdoor emergency lights on Taobao. They have rather limited functions (only a fluorescent lamp) and are priced quite high. Coincidentally, I had acquired some ultra small 8 pin microcontrollers, which were just sitting idle, so I decided to make one on my own.
Normally, it can be attached to a keychain. In case of an emergency stop at night, it can be turned on with a strobe function and placed behind the car as a warning. It can also serve as a flashlight. When going hiking, hanging it on the backpack and activating the fluorescent energy saving mode, it will flash like a firefly, preventing fellow hikers from losing track of each other (or making it easier for oneself to be located if lost).
##Functions
1. Long Battery Life
* Continuous on mode: 2 hours.
* 5 second strobe mode: 24 hours.
* Extended duration mode (15 second strobe): 3 days.
* Battery leakage is in the nA range when powered off, enabling long term storage.
2. Multiple Functions
* Long lasting Fluorescent Lamp: High brightness phosphors are excited periodically by ultraviolet light, achieving a good lighting effect with extremely low power consumption. It supports three modes: continuous on, 5 second strobe, and 15 second strobe.
* White light Flashlight: Offers three modes: continuous on, 1 second strobe, and 5 second strobe.
* Orange Fog Lamp: Features three modes: continuous on, 1 second strobe, and 5 second strobe.
* Red Warning Light: Has three modes: continuous on, 1 second strobe, and 5 second strobe.
* Anti accidental Startup: It requires a long press followed by four short presses to start. In case of accidental touches, a strobe indication will be given to prevent accidental activation when in the pocket.
3. Outdoor oriented Design
* Fully sealed and potted for dust proof and waterproof performance.
## Design Details
1. The dimensions are 35mm * 35mm * 30mm. It uses an LR2477 button cell battery with a nominal capacity of 190mAh and a discharge current of 100mA. 2. Only one LED (60mA) can be illuminated at a time. If more LEDs need to be lit, parallel connection with another battery should be considered.
It adopts a classic design of TP4056 charging, PW5410B charge pump for buck boost to 3.3V, and USB battery switching. Additionally, a dual MOSFET design is added to prevent the battery from leaking current to TP4056, thus prolonging the storage time.
3. An 8 pin STC8G17A microcontroller combined with a TCA9534A for IO expansion is used. These components are robust and come with a built in low voltage detection function. Domestic low end chips prove to be quite powerful.
4. It is potted with epoxy resin glue twice and requires a fair amount of manual work such as various polishing processes. For mass production, a 3D printed shell can be considered.