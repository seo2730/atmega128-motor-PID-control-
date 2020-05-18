# YouTube
Speed controller : https://www.youtube.com/watch?v=1ZMwWaizCjQ <br>
Position controller : https://www.youtube.com/watch?v=dzYhXpqx9Co <br>

# DC Motor Modeling
![image](https://user-images.githubusercontent.com/42115807/82224695-23377000-995f-11ea-8ff5-b5cbced47643.png)
Te: 토크, b: 마찰 계수, J: 관성, 𝜽𝒎: 각도

### No load system 
>𝑻𝒆 - 𝒃 x d(𝜽𝒎)/dt ̇= 𝑱 x d(𝜽𝒎)/dt 

### Load system
>𝑻𝒆 - 𝒃 x d(𝜽𝒎)/dt - 𝑻𝑳  ̇= 𝑱 x d(𝜽𝒎)/dt

### DC Motor 전기 방정식
![image](https://user-images.githubusercontent.com/42115807/82225564-52021600-9960-11ea-8a37-4633cb98c934.png)
Va : 총 전압, Ra: 저항, La: 인덕턱스, ea: 유기 기전력<br>

>Va = Ra x ia + La x d(ia)/dt + ea

### Transfer Function(전달함수)

# Geared Motor Modeling
![image](https://user-images.githubusercontent.com/42115807/82223857-2ed66700-995e-11ea-937a-9002f202ff3e.png)
### 모터 운동 방정식
T: 총 토크 , TM: 모터 토크, TGM: 부하 토크 <br>
JM: 모터 관성, BM: 모터 마찰계수, JGM: 부하 관성, BGM: 부하 마찰계수<br>
𝜽𝑴: 모터 각도
>𝑻 = 𝑻𝑴 + 𝑻𝑮𝑴 = 𝑱𝑴 x d^2(𝜽𝑴)/dt^2 + 𝑩𝑴 x d(𝜽𝑴)/dt + 𝑱𝑮𝑴 x d^2(𝜽𝑴)/dt^2 + 𝑩𝑮𝑴 x d(𝜽𝑴)/dt

### 기어 운동 방정식
>𝑻𝑮 = 𝑱𝑮 x d^2(𝜽𝑮)/dt + 𝑩𝑮 x d(𝜽𝑮)/dt

# PID

# Code
