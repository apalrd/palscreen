echo 252 > /sys/class/gpio/export
echo 'out' > /sys/class/gpio/gpio252/direction
echo '1' > /sys/class/gpio/gpio252/value