## Drone styring program

Tello::Video

- Recieve encoded video signal
- Decode and apply to SFML texture



Tello::Detection

- For different detection algorithms that can highlight things in the viewport or steer the drone, eg. chasing down redheads and tasing them.



Tello::UI

- Setup of SFML
- Display drone telemetry like speed, height, battery and other stuff. Perhaps debug console for viewing which data has been sent and what has been received, along with input for sending custom commands
- Display video and allow for recording video and taking images
- Enable different detection algorithms and allow them to take control of the drone
- Drone control by keyboard



Tello::API

- All of the api calls, including setting and recieving speed...
- Initialization and connection to the drone



Tello::UDP

- Here we run a server on another thread, where we handle communications
- Server setup:
  - There is a queue
  - When we send a command to the drone, an ID for the request will be made an that ID will be returned immediately.
  - A response server, that is run on another thread will continuously check for responses from the drone.
  - The next response from the drone will be coubled to the ID from the latest command sent. That pair will then be saved in memory, that can later be accessed.

- Video
  - Video will also be handled in the UDP class
  - A thread will be created for recieving video frames from the drone, which will be saved in a framebuffer that can be accessed by Tello::Video
  - 



