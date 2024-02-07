# Computer Graphics

Contains all the assignments of the course Computer-Graphics. 

## OpenGL Installation (linux)
```sh
  sudo apt update && sudo apt upgrade
  sudo apt install freeglut3-dev
```

## Running openGL in VSCode
- Install the C/C++ extension by Microsoft
- Install the Code Runner extension by Jun Han
- Hit the settings icon and select the correct cpp compiler
- Add the following lines in the tasks.json file that is created in the `.vscode` folder

```js
  "args": [
    "-lGL",
    "-lGLU",
    "-lglut"
  ],
```

## Running openGL in terminal:
```sh
  g++ <filename.cpp> -o <outputFileName> -lGL -lGLU -lglut
  ./<outputFileName>
```