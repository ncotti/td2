# td2

# TODO

# Pie chart
# Debug Motor controllers
# Add text label on image displayer (Ok: {frame_number})
# Glue columns
# Reprint cylinder (?)

# Useful
python3 train.py --img 640 --rect --data training/eraser_training/data.yaml --cache ram

python3 train.py --img 640 --rect --data training/galletas_640/data.yaml --cache ram

python3 train.py --img 640 --rect --data training/rumba_y_anillo/data.yaml --cache ram

python3 detect.py --weights best.pt --source 0 --imgsz 160