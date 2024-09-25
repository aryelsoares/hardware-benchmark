# Information

This project aims to create an observability system for real-time monitoring of hardware resources using C++ for data collection, InfluxDB for storage and Grafana for data visualization. The process is done automatically using docker containers. It collects eight data types per second as showed below.

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

# Requirements

- **Package:**
  - git
  - lm-sensors
  - docker
  - docker-compose

The functionality is made exclusively for personal use, taking into account the author's computer hardware. Some adjustments to the files will be required to make them work on different computers. This project is applied to the machine with the following specifications:

- **Specification:**
  - OS: Arch Linux
  - CPU: Ryzen 5 5600G
  - GPU: RX 6750XT
  - RAM: 2x16GB

The software tool **lm-sensors** is required in order to extract temperature data. It's important to have previously used sensor detection so *temp_info.hpp* can properly get data. By default, the command is ```sensors-detect``` where all analysis suggestions should be denied. After that just use ```sensors``` to get something like below.

- **k10temp-pci-00c3**
  - Adapter: PCI adapter
    - Tctl:         +26.2°C
- **nvme-pci-0a00**
  - Adapter: PCI adapter
    - Composite:    +40.9°C
    - Sensor 1:     +40.9°C
    - Sensor 2:     +26.9°C
- **amdgpu-pci-0300**
  - Adapter: PCI adapter
    - edge:         +35.0°C
    - junction:     +35.0°C
    - mem:          +46.0°C
- **nvme-pci-0700**
  - Adapter: PCI adapter
    - Composite:    +39.9°C

Temperature data is obtained as subfeature and the processing order is relevant for analysis, including quantity. By default, both nvme-pci sensors will be ignored.

# Instructions

_**NOTE**_: If directories for data extraction or the temperature list are wrong, the service ends with an error.

To extract data and monitor it in real time, simply run the containers using docker compose. First clone this repository with the following command:

```bash
git clone https://github.com/aryelsoares/Hardware-Benchmark.git
```

Change current directory to the same where it was cloned. After that run the following 
command to start it:

```bash
docker-compose up --build
```

After that data extraction is made automatically. You can check the dashboard with Grafana 
browsing ```127.0.0.1:3000```. Both user and 
password are **admin**.

Remember to finish access later on with:

```bash
docker-compose down
```

# Notes

Although this project is applicable for a specific computer, its use can be adjusted for other computers by making appropriate changes to the files. Its main use is to monitor hardware usage in real-time.