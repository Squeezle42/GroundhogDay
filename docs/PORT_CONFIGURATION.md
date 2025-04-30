# Browser-Sync Port Configuration

This document explains the ports used by the development servers in this project.

## Default Port Configuration

By default, Browser-Sync uses two ports:

1. **Port 5000 - Main Content Server**
   - Serves the actual game/website content
   - This is the URL you should use to access your game: http://localhost:5000
   - All HTML, CSS, JavaScript, and assets are served from this port

2. **Port 5001 - Browser-Sync UI (Admin Interface)**
   - Provides a web-based control panel for Browser-Sync
   - Only needed for advanced development tasks
   - Can be disabled with the `--no-ui` flag

## Current Configuration

The current setup is configured to:

- Use port 5000 for the main content
- Disable the UI interface on port 5001 (using the `--no-ui` flag)

## Available npm Scripts

- `npm run start` - Starts the server without UI on port 5000 (basic configuration)
- `npm run start-dev` - Starts the server without UI with extended file watching (recommended for development)
- `npm run start-with-ui` - Starts the server with UI interface enabled (if you need the admin panel)

## Manual Port Configuration

If you need to change the port (e.g., if port 5000 is already in use), you can:

1. Edit the port number in `package.json` scripts
2. Edit the port number in `dev_server.bat` and other batch files

Example:
```json
"start-dev": "browser-sync start --server --port 3000 --files \"*.html, styles/**/*.css, scripts/**/*.js, assets/**/*, *.py, *.md\" --no-notify --browser \"chrome\" --startPath \"index.html\" --no-ui"
```

## Troubleshooting Port Issues

If you encounter port conflicts:

1. Check what's using the port:
   ```
   netstat -ano | findstr "5000"
   ```

2. Find the process using the port:
   ```
   tasklist /FI "PID eq [PID-FROM-NETSTAT]"
   ```

3. Kill the process if needed:
   ```
   taskkill /F /PID [PID]
   ```

4. Or simply change the port number in the configuration files
