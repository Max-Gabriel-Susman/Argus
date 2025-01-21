# Argus Troubleshooting

## Issue: bind() to 0.0.0.0:1935 failed (98: Address already in use)

sudo lsof -i :1935

sudo kill <PID>