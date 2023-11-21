# AlloyCore
AlloyCore is the core of the Alloy Game Engine, a simple, modular, and performant engine written in C++20, based on the ECS architecture. AlloyCore itself does not provide much for creating games. Rather, applications must make use of plugins to add functionality to the engine. This core library is the base of all Alloy Game Engine plugins and applications.

# Concepts
## Applications
Applications are defined as a set of plugins which work together to achieve the wanted result. The application is just the glue which holds plugins together, and does not contain any game logic. By this definition, your game must be defined within plugins, and the application purely runs these plugins which define your game.

## Plugins
Plugins provide functionality to the engine, whether that be a rendering solution, a physics simulation, or the logic of your game. To do this, plugins are made up of systems which are called by the application at the appropriate times. These systems interact with resources and components, which are the data of your application, and which are also defined by plugins. For example, a rendering plugin may provide a mesh component, which it reads from every frame to render your world, and a window plugin may provide a resource which defines your window, and allows other plugins to interact with it.

## Systems
Systems are functions which can only accept component queries or resource reads/writes as parameters. These make up the logic of the application.

## Components
Components are pure data which can be associated to an entity. There can be many components of the same type in your world.

## Resources
Unlike components, resources are unique pieces of data which can be accessed globally in your application.

