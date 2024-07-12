# Sabida: Smart Plant Watering System

## Overview

Sabida is a smart plant watering system that leverages .NET MAUI Blazor for mobile application development and an ESP-12F microcontroller for IoT integration. The mobile app allows users to control the watering schedule, set watering durations, and manually turn the watering pump on or off at any time. This project demonstrates the powerful combination of .NET MAUI, Blazor, MudBlazor, and Refit to create a seamless IoT solution.

## Features

- **Cross-Platform Mobile App**: Built using .NET MAUI Blazor, the app runs on Android, iOS, and Windows.
- **Intuitive UI**: Leveraging MudBlazor for a rich set of customizable UI components.
- **Real-Time IoT Control**: Integrates with an ESP-12F microcontroller to control a watering pump.
- **Flexible Scheduling**: Set specific days and times for watering.
- **Manual Override**: Turn the watering pump on or off at any time.
- **Extensible Architecture**: Easily add more devices with similar logic.

## Technologies Used

- **.NET MAUI**: For cross-platform mobile application development.
- **Blazor**: To build interactive web UIs using C#.
- **MudBlazor**: For UI components.
- **Refit**: A type-safe REST client for .NET.
- **ESP-12F**: Microcontroller programmed with Arduino for IoT functionality.
- **Arduino**: Platform for programming the ESP-12F.

## Getting Started

### Prerequisites

- .NET 7 SDK or later
- Visual Studio 2022 or later (with .NET MAUI workload installed)
- Arduino IDE
- ESP-12F microcontroller
- Basic knowledge of C# and Arduino programming

### Setting Up the ESP-12F

1. **Install the ESP8266 Board Package**:
   - Open the Arduino IDE.
   - Go to `File > Preferences`.
   - Add the following URL to the "Additional Boards Manager URLs" field: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`.
   - Go to `Tools > Board > Boards Manager`, search for `ESP8266`, and install it.

2. **Upload the Code to ESP-12F**:
   - Open `AllSmart.Arduino/WebServer.ino` from the `Arduino` directory in the repository.
   - Configure your WiFi SSID and password in the `secrets.h`.
   - Connect your ESP-12F to your computer and select the appropriate board and port in the Arduino IDE.
   - Upload the code to the ESP-12F.

3. **Device Diagram**:
   - Refer to the device diagram provided in the `Diagrams` directory of the repository to correctly wire your ESP-12F microcontroller with the necessary components for the watering system.

### Setting Up the Mobile App

1. **Restore NuGet Packages**:
   - Open the `Sabida.sln` solution in Visual Studio.
   - Restore the NuGet packages by building the solution.

2. **Run the App**:
   - Select the target platform (Android, iOS, or Windows) and run the app.

## Usage

- **Scheduling Watering**: Use the app to set specific days and times for the watering pump to activate.
- **Manual Control**: Override the schedule to turn the pump on or off at any time.
- **Monitoring**: View the current status of the watering pump and upcoming watering schedules.

## Extending the Project

The architecture of the mobile app allows for easy extension to support additional IoT devices. By sharing the same ViewModel between MAUI and Blazor components, you can maintain a consistent development process and reuse code effectively.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [MudBlazor](https://mudblazor.com/)
- [Refit](https://github.com/reactiveui/refit)
- [Arduino](https://www.arduino.cc/)
- [ESP8266](https://www.espressif.com/en/products/hardware/esp8266ex/overview)

---
![Screenshot_2024-07-12-17-55-35-768_com secalhar sabida (1)](https://github.com/user-attachments/assets/e09ff6bd-748e-4b53-ba84-54e124c57820)

![Screenshot_2024-07-12-17-56-44-745_com secalhar sabida](https://github.com/user-attachments/assets/5dc348d2-8eb2-43e8-a9fe-67853d822275)


### Connect with Me

- **LinkedIn**: [Fernando Silva Jr.](https://www.linkedin.com/in/nandosilvajr/)
- **GitHub**: [nandosilvajr](https://github.com/nandosilvajr)



Happy coding!
