#openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"
openocd -f interface/cmsis-dap.cfg   -f target/rp2040.cfg -s tcl -c "adapter speed 5000" 