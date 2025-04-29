# Time Loop Game - Deployment Documentation

## Credits and Attributions

For a comprehensive list of all sources, inspirations, tools, and attributions, please see our dedicated [CREDITS.md](CREDITS.md) file.

### Special Bill Murray Note
If Bill Murray ever finds this game and wants to stop by, he's officially invited to the developer's birthday party, free of charge! No RSVP needed - just show up!

## Game Structure

### Current Implementation

Our current version of the game includes:

1. **Beginning** - A complete intro sequence where the player wakes up to realize they're in a time loop
2. **End** - A complete ending sequence with credits and acknowledgments

The middle portion of the game will be implemented in future releases. This development approach allows us to test the core mechanics and narrative framing before investing in the full gameplay loop.

## Deploying to Google Cloud Hosting

### Pre-requisites
- A Google Cloud Platform account
- Google Cloud SDK installed
- A built and packaged version of the game

### Deployment Steps

1. **Create a Google Cloud Storage bucket**
   ```bash
   gsutil mb -l us-central1 gs://your-timeloop-game-bucket
   ```

2. **Set bucket permissions to public**
   ```bash
   gsutil iam ch allUsers:objectViewer gs://your-timeloop-game-bucket
   ```

3. **Enable website configuration**
   ```bash
   echo '{"website": {"mainPageSuffix": "index.html"}}' > website.json
   gsutil web set -m index.html gs://your-timeloop-game-bucket
   ```

4. **Upload the game files**
   ```bash
   gsutil -m cp -r ./PackagedGame/* gs://your-timeloop-game-bucket/
   ```

5. **Set appropriate CORS configuration**
   ```bash
   echo '[{"origin": ["*"], "method": ["GET"], "responseHeader": ["Content-Type"], "maxAgeSeconds": 3600}]' > cors.json
   gsutil cors set cors.json gs://your-timeloop-game-bucket
   ```

6. **Access your game**
   Your game will be available at: https://storage.googleapis.com/your-timeloop-game-bucket/index.html
   
7. **Optional: Set up a custom domain**
   For a custom domain, you can set up Google Cloud Load Balancer with your bucket as a backend.

### Continuous Deployment

You can set up GitHub Actions or Google Cloud Build to automatically deploy updates:

```yaml
name: Deploy to Google Cloud Storage

on:
  push:
    branches: [ main ]

jobs:
  deploy:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    
    - name: Setup Google Cloud SDK
      uses: google-github-actions/setup-gcloud@v0
      with:
        project_id: ${{ secrets.GCP_PROJECT_ID }}
        service_account_key: ${{ secrets.GCP_SA_KEY }}
        export_default_credentials: true
    
    - name: Build the game
      run: |
        # Add your build commands here
    
    - name: Deploy to Google Cloud Storage
      run: |
        gsutil -m cp -r ./PackagedGame/* gs://your-timeloop-game-bucket/
```

## Development Roadmap

1. **Phase 1 (Current)**: Beginning and end sequences
2. **Phase 2**: Core town exploration and NPC interactions
3. **Phase 3**: Time loop mechanics and memory persistence
4. **Phase 4**: Main storyline and puzzle implementation
5. **Phase 5**: Side quests and full town activities
6. **Phase 6**: Polish, optimization and release

## Contributors

- Lead Developer: [Your Name]
- Game Design: Inspired by "Groundhog Day" (1993)
- Music: "Rocket Man" as performed by Commander Chris Hadfield
- Special Thanks: Bill Murray, for the timeless inspiration
