# Detección de objetos sobre línea de montaje

## Ejecución

Clonar el repositorio con submódulos:

```
$ git clone https://github.com/ncotti/td2.git
$ git submodule init
$ git submodule update
```

Ejecutar la interfaz gráfica:
```sh
$ python3 gui.py
```

## Entrenamiento de la IA

Se debe crear un proyecto en [Roboflow](https://roboflow.com/), exportar el proyecto dentro de la carpeta `yolov5/training/<project_name>`, modificar los paths dentro del archivo `data.yaml` para que coincidan con los paths locales como sigue:

```yaml
train: ./training/<project_name>/train/images
val: ./training/<project_name>/valid/images
test: ./training/<project_name>/test/images
```

Y luego ejecutar:

```sh
$ cd yolov5

$ python3 train.py --img 640 --rect --data training/<project_name>/data.yaml --cache ram
```

## Árbol de directorios

```
td2
├── camera
├── widgets
├── weights
├── gui.py
├── uart_rx.py
├── uart_tx.py
└── const.py
```

* **camera/**: Proyecto de STM32CubeIDE para cargar sobre el microcontrolador (se usa la placa de desarrollo STM32F401RE).

* **widgets/**: Todos los widgets usados por la interfaz gráfica de QT.

* **weights**: Todos los resultados del entrenamiento de la inteligencia artificial.

* **ncotti/yolov5**: Submódulo con el algoritmo de yolov5 personalizado para los requerimientos del proyecto (repositorio aparte).

* **gui.py**: Archivo que inicializa la interfaz gráfica.

* **uart_rx.py | uart_tx.py**: Lectura y escritura de la UART.

* **const.py**: Archivo de constantes del projecto.

## Autor
Hecho por Nicolas Gabriel Cotti, como proyecto de Técnicas Digitales 2.

Diciembre, 2022.


# Useful
python3 train.py --img 640 --rect --data training/eraser_training/data.yaml --cache ram

python3 train.py --img 640 --rect --data training/galletas_640/data.yaml --cache ram

python3 train.py --img 640 --rect --data training/rumba_y_anillo/data.yaml --cache ram

python3 train.py --img 640 --rect --data training/rumba_y_anillo_v2/data.yaml --cache ram

python3 detect.py --weights best.pt --source 0 --imgsz 160