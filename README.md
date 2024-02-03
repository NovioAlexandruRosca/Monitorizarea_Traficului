# Project Title: Traffic Monitoring System with GTK3 Integration

## Creation Interval.
November 20, 2023-January 10, 2024

## Description

Introducing "Traffic Buddy," a project led by Rosca Alexandru-David crafted with passion and ingenuity to streamline your daily commute. Developed using C for Linux, "Traffic Buddy" is your go-to companion for navigating city streets with ease and safety.
"Traffic Buddy" operates on a simple yet effective client-server model, allowing seamless communication between users and our server. With just a few clicks, you can access real-time data and services to enhance your travel experience.
"Traffic Buddy"'s goal is to empower students and commuters with practical tools for safer and more efficient travel. By leveraging technology and teamwork, we aim to make a positive impact on everyday journeys.

## Features

- **Live Weather Updates**: Stay ahead of the weather curve with real-time forecasts tailored to your route.
- **Gas Price Comparison**: Find the best fuel deals at nearby gas stations, helping you save on your travels.
- **News Highlights**: Stay in the loop with the latest headlines, keeping you informed on the go.
- **Custom Speed Recommendations**: Get personalized speed tips based on street conditions, keeping you safe and compliant.
- **Interactive City Map**: Navigate with confidence using our user-friendly map interface, pinpointing your exact location and nearby landmarks.
- **Speed Monitoring**: Stay on track with our speed monitoring feature, ensuring you're driving safely within the limits.
- **Accident Reporting**: Report incidents quickly and efficiently, alerting fellow users and emergency services for prompt assistance.
- **Sleek GTK3 Interface**: Enjoy an intuitive and modern interface designed with GTK3, making navigation a breeze.

And many more other features.

## How To Run
- **For the server side**: 
sudo apt-get update
sudo apt-get install sqlite3 libsqlite3-dev

gcc server.c -o server -lsqlite3

- **For the client side**: 
sudo apt-get install libgtk-3-dev

gcc client.c -o client pkg-config --cflags --libs gtk+-3.0

## Screenshots

Here you can see a couple of screenshots of the app:

- **Main App**: The main interface of the application.
![Exemplu_rulare](https://github.com/AlexandruRoscaPOO/Monitorizarea_Traficului/assets/113398639/75561fbd-7e1c-45cc-8752-bf9af9a1f274)

- **Concurrency Feature**:
![Exemplu_rulare_concurent](https://github.com/AlexandruRoscaPOO/Monitorizarea_Traficului/assets/113398639/7b43e7f6-8e00-411c-821b-8f74c331bc52)

- **Manual Speed Modification**: You are able to set a specific speed that will be computed by the server and will let you know if you are driving with a safe speed.
![Modificare_viteza_1](https://github.com/AlexandruRoscaPOO/Monitorizarea_Traficului/assets/113398639/d8dcefdd-0c53-4aa7-91e7-11fda55d35d2)
![Modificare_viteza_2](https://github.com/AlexandruRoscaPOO/Monitorizarea_Traficului/assets/113398639/f6a69347-19d0-4551-9fc7-881af57765d2)
- **The Server**: Will give you important information about the time a request was received and the data that has been sent back to the user
![Server](https://github.com/AlexandruRoscaPOO/Monitorizarea_Traficului/assets/113398639/37cbd3ff-5c23-4583-a2bc-4a8dfaf4a826)
- **The Client**:  Will send requests to the server periodically or on-action and will await a response without breaking the flow of the app
![Client](https://github.com/AlexandruRoscaPOO/Monitorizarea_Traficului/assets/113398639/b5648c47-d15f-4990-9ba6-456a8d7118a7)
- **Example of the news feature**: You will get new news periodically if you select the option(same for weather and prices)
![Stire_sportiva](https://github.com/AlexandruRoscaPOO/Monitorizarea_Traficului/assets/113398639/52a62c6b-6503-4f5b-98dc-4b0f76d5b6d0)
- **Accident Button**: If you recall any accidents on the road that you are driving on let everybody know it
![utilizare_accident](https://github.com/AlexandruRoscaPOO/Monitorizarea_Traficului/assets/113398639/2fb03abc-b593-4bb9-8635-47a7d5a6ae5c)



