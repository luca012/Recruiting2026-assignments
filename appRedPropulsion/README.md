# Zephyr STM32 Nucleo-H723ZG App

This repository contains a minimal Zephyr application configured for the STM32 Nucleo-H723ZG. It is structured as a **Zephyr Workspace Application**, meaning this repository acts as the manifest source.

## Setup Instructions
>we recommend developing in a linux enviroment (the following isntructions are for a UNIX system) but use what you have.

To build and run this application, you need to use `west` to initialize a workspace around it. This will automatically pull down the Zephyr kernel and the required STM32/ARM CMSIS dependencies.

### 1. Directory setup
```bash
mkdir your-zephyr-workspace
cd your-zephyr-workspace

```

### 2. Venv
```bash
python -m venv .venv

source .venv/bin/activate
```
if using `uv` instead of plain `pip`

```bash
uv venv

source .venv/bin/activate
```

### 3. Install Dependencies
Ensure you have the [Zephyr SDK and West installed](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) on your system.
or you can use:

```bash
pip install west
```
or
```bash
uv pip install west
```

### 4. Create a Workspace & Clone
Create an empty directory for your workspace, then clone this repository inside it:

```bash
git clone <YOUR_GITHUB_REPO_URL> my_app
```

### 5. Update dependencies

```bash
west zephyr-export
west packages pip --install # or west packages pip | xargs uv pip install 
west sdk install --toolchain arm-zephyr-eabi
```

## Develop your app
Based on the project assigned to you develop everything in the `src` dir, create other files if needed, you may or may not have to edit the `prj.conf` and `nucleo_h723zg.overlay` files.
> note: `.overlay` file is an overlay for `.dts` aka device tree files.

you may contact who assigned you the project for assistance.

to submit the project make sure it compiles.

## Bulding your app
```bash
west build apps/my_app
```
