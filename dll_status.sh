PID=$(pgrep -n -f 'Anno1800.exe')
echo "PID:"
echo "$PID"
echo ""

grep -E 'd3d11|d3d12|d3d11on12|dxgi' /proc/$PID/maps

echo ""
echo "Mod Dlls:"
echo ""

grep -E 'TextShaping|Anno1800ModMenu|version(_orig)?\.dll' /proc/$PID/maps

