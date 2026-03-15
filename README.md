# ℹ️ Information

This project aims to create an observability system for real-time monitoring of hardware resources using **Rust** for data collection, **InfluxDB** for storage and **Grafana** for data visualization. The process is done automatically using docker containers.

## ✨ Features

It collects nine data variants per second as showed below.

- **CPU**
  - Usage
  - Temperature
  - Power
- **GPU**
  - Usage
  - Temperature (edge)
  - Temperature (junction)
  - Temperature (mem)
  - Power
- **RAM**
  - Usage

The data can se analysed through **Hardware Info** dashboard.

![benchmark](https://github.com/user-attachments/assets/b87a0a71-5c33-41d5-acc4-c05039f89ff5)

## ⚙️ Requirements

- **Package:**
  - git
  - docker
  - docker-compose

The functionality is made exclusively for personal use, taking into account the author's computer hardware. Some adjustments to the files will be required to make them work on different computers. This project is applied to the machine with the following specifications:

- **Specification:**
  - OS: Arch Linux
  - CPU: Ryzen 5 5600G
  - GPU: RX 6750XT
  - RAM: 2x16GB

## 🚀 Instructions

To extract data and monitor it in real time, simply run the containers using docker compose. First clone this repository with the following command:

```bash
git clone https://github.com/aryelsoares/hardware-benchmark.git
cd hardware-benchmark
```

Change current directory to the same where it was cloned. After that run the following command to start it:

```bash
docker-compose up --build
```

After that data extraction is made automatically. You can check the dashboard with Grafana browsing ```127.0.0.1:3000```. Both user and password are **admin**.

Remember to finish access later on with:

```bash
docker-compose down -v
```

## 📝 Notes

- Although applicable for a specific computer, its use can be adjusted for other computers by making appropriate changes to the files. Its main use is to monitor hardware usage in real-time.
- This project was made in **C++** and then refactored in **Rust**.
