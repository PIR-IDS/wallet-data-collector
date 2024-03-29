# PIR – Wallet Data Collector for Arduino

<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/PIR-IDS/wallet-data-collector">
    <img src="https://avatars.githubusercontent.com/u/99486891" alt="Logo" width="130">
  </a>

  <p align="center">
    IDS: Code to collect the data for the wallet model
    <br />
    <a href="https://github.com/PIR-IDS/wallet-data-collector/releases"><strong>See Releases »</strong></a>
    <br />
    <br />
    <a href="https://github.com/PIR-IDS/research-paper">Research Paper</a>
    ·
    <a href="https://github.com/PIR-IDS/wallet-data-collector/actions/workflows/test.yml">Test Results</a>
    ·
    <a href="https://github.com/PIR-IDS/.github/blob/main/profile/README.md#usage">See Global Usage</a>
  </p>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#execution">Execution</a></li>
        <li><a href="#generation">Generation</a></li>
        <li><a href="#tests">Tests</a></li>
        <li><a href="#documentation">Documentation</a></li>
      </ul>
    <li><a href="#tree-structure">Tree Structure</a></li>
    <li><a href="#credits">Credits</a></li>
    <li><a href="#contact">Contact</a></li>

  </ol>
</details>

***

<!-- ABOUT THE PROJECT -->
## About The Project

This code will be used in order to detect anomalies and to inform our IDS about it.

### Built With
* [C++](https://isocpp.org/)
* [Arduino Framework](https://docs.platformio.org/en/stable/frameworks/arduino.html)

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

1. Install the latest version of Python 3 with `pip` (included): https://www.python.org/downloads/
2. Install the latest version of PlatformIO: https://docs.platformio.org/en/latest//core/installation.html#installation-methods
    ```sh
    pip install -U platformio
    ```

### Installation

1. Clone the project
   ```sh
   git clone https://github.com/PIR-IDS/wallet-data-collector.git
   ```
2. Install the dependencies by typing the following command while being in the project root:
   ```sh
   pio pkg install
   ```

<details>
  <summary>Using an IDE</summary>
  <br />
  
  If you want to use a specific IDE with this project, you can list the compatible ones by checking the `--ide` line with:
  
  ```sh
  pio init -h
  ```

  Choose your IDE with:
  ```sh
  pio init --ide <ide>
  ```

  > :warning: **DO NOT COMMIT THE NEWLY CREATED FILES AFTER THE IDE INIT COMMAND.**
  > 
  > You can add their name/location in the `.git/info/exclude` file to exclude them from the VCS without affecting the `.gitignore` file. You will also have to add the build directories created by your IDE in order to keep your artifacts outside of the project repository.

</details>

<!-- USAGE EXAMPLES -->
## Usage

### Execution

Use the following command in the project root to build and upload the firmware in the default `debug` mode (**with the board plugged**):
  ```sh
  pio run -t upload
  ```

By adding the `-e release` option you will be in `release` mode.

### Generation

You can also build the firmware in the default `debug` mode without uploading:
  ```sh
  pio run
  ```

Again, by adding the `-e release` option you will be in `release` mode.

### Tests

Before running the tests on your machine, you need to follow these instructions: https://docs.platformio.org/en/latest/platforms/native.html#installation

Then, you can run the "desktop" (i.e. not necessitating the board) tests with the following command:
  ```sh
  pio test -e native_debug -e native_release
  ```

If you want to run the tests for the board, you need to run this command with the board plugged:
  ```sh
  pio test -e debug -e release
  ```

### Documentation

Before generating the documentation on your machine, you need to follow these instructions: https://doxygen.nl/manual/install.html

Then, to generate the Doxygen documentation, run the following commands:
  ```sh
  mkdir -p build/docs
  doxygen
  ```

#### How to collect the data?
  
To collect properly the data of the accelerometer without having to plug the card to a PC, we use the bluetooth connection by running the [BLE Reader](https://github.com/PIR-IDS/ble-reader) on a PC to retrieve the accelerometer data. After uploading the code on the arduino, unplug it, start the battery and put the arduino in the wallet you want to test. Then, record and retrieve the data by running the BLE Reader on a laptop.
  
For the positive samples, edit the main.cpp file to send 500 lines on each sample.
  
For the negative samples, there are two options. First, we can record the data of the wallet utilisation without pay card utilisation. To do that, the method is similar as the one for the positive samples. We can also record the data of the wallet during non-wallet utilisation (walking, climbing stairs, ...). To do that, edit the main.cpp file to send 2000 lines on each sample.
  
The data of the accelerometer is collected by the arduino between 2 transmissions. The moves done during transmission are not recorded.

***

<!-- TREE STRUCTURE -->
## Tree Structure
<details>

_TODO_

</details>

<!-- CREDITS -->
## Credits

Romain Monier [ [GitHub](https://github.com/rmonier) ] – Co-developer
<br>
Noé Chauveau [ [GitHub](https://github.com/Noecv) ] – Co-developer

<!-- CONTACT -->
## Contact

Project Link : [https://github.com/PIR-IDS/wallet-data-collector](https://github.com/PIR-IDS/wallet-data-collector)

Organization Link : [https://github.com/PIR-IDS](https://github.com/PIR-IDS)
