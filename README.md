# 🛠️ Bare-Metal STM32 Driver Layer & Protocol Management
> **CMSIS Register-Level | Zero-HAL | Mission-Critical**

[![Platform: STM32](https://img.shields.io/badge/Platform-STM32-blue.svg)](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html)
[![Architecture: ARM Cortex-M](https://img.shields.io/badge/Arch-ARM%20Cortex--M-orange.svg)](https://developer.arm.com/ip-products/processors/cortex-m)
[![Language: C](https://img.shields.io/badge/Language-C-green.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Compliance: MISRA-C](https://img.shields.io/badge/Compliance-MISRA--C%3A2012-red.svg)](#-testing--cicd)

A high-reliability, zero-HAL **bare-metal driver layer** and in-board protocol management architecture for ARM Cortex-M microcontrollers. Developed by referencing Datasheets and Reference Manuals directly to achieve low-level hardware control without manufacturer abstraction libraries.

---

## 🚀 Features

- **Zero-HAL Control** – Direct register access via `volatile struct*` CMSIS memory mapping.
- **Deterministic Execution** – No dynamic memory usage; fast, low-overhead peripheral access.
- **Interrupt & NVIC Management** – Non-blocking ISR design with explicit flag clearing.
- **Protected Telemetry Protocol** – Packed structures transmitted over UART with CRC-16 validation.
- **State Machine Drivers** – Robust SPI and I2C hardware drivers with bus-busy and error protection.

---

## 📂 Project Structure

```text
baremetal-stm32-driver-layer/
├── drivers/
│   ├── inc/       # Register definitions & driver public headers
│   └── src/       # Low-level driver implementations (RCC, GPIO, USART, SPI, I2C)
├── app/           # Application layer & telemetry task implementation
├── tools/         # Python integration tests (PySerial & CRC-16 verification)
├── docs/          # Doxygen architectural design documentation
├── Makefile       # Cross-compilation & automated check rules
└── .github/       # CI/CD workflows for compilation & static analysis
```

---

## 📐 Requirements & Architecture

This project is built from the ground up for safety-critical, high-determinism environments:

*   **Register Abstraction**  
    Custom peripheral base-address maps and bit-field definitions using strict volatile pointer types.
*   **Clock Tree Control**  
    Bare-metal RCC configuration for core system and peripheral buses to guarantee precise execution timing.
*   **Safe Communications**  
    SPI IMU & I2C sensor interfaces built with non-blocking error guards, timeout loops, and recovery routines.
*   **Packed Structures**  
    Explicit byte alignment using `__attribute__((packed))` to ensure binary frame compatibility with downstream host machines.
*   **Doxygen Documentation**  
    Architectural design compliant with V-Model Software Development standards.

### Zero-HAL Register Abstraction Pattern

```c
/* Direct memory mapping of GPIO Registers */
typedef struct {
    volatile uint32_t MODER;   /* GPIO port mode register        */
    volatile uint32_t OTYPER;  /* GPIO port output type register */
    volatile uint32_t OSPEEDR; /* GPIO port output speed register*/
    volatile uint32_t PUPDR;   /* GPIO port pull-up/pull-down    */
    volatile uint32_t IDR;     /* GPIO port input data register  */
    volatile uint32_t ODR;     /* GPIO port output data register  */
    volatile uint32_t BSRR;    /* GPIO port bit set/reset register*/
} GPIO_TypeDef;

#define GPIOA_BASE (0x40020000UL)
#define GPIOA      ((GPIO_TypeDef *) GPIOA_BASE)
```

---

## 🧪 Testing & CI/CD

High reliability is enforced through static analysis and target loopback simulations:

*   **Integration Testing** – Python-based real-time telemetry verification via serial interface.
*   **Data Integrity** – Real-time CRC-16 frame validation on host PC.
*   **CI/CD Pipeline Runs**
    *   Strict GCC cross-compiler checks (`-Wall -Wextra -Werror`).
    *   Static Code Analysis enforcing MISRA rules (using `cppcheck`).
    *   Automated compilation and build verification.

---

## 🎯 Applications

The design patterns implemented in this layer are ideal for:

*   🚀 **Aerospace & Defense Avionics**
*   🎮 **Flight Control Computers (FCC)**
*   ⚡ **High-Performance Embedded Controllers**
*   🚗 **Automotive Electronic Control Units (ECU)**