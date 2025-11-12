# 🪂 Project Leap — Unreal Engine 5 Platformer Prototype

**Project Leap** is a 3D platformer prototype built in **Unreal Engine 5**, developed entirely from scratch in **C++** while reusing template animations for rapid iteration.  
The goal of this project is to design and implement a responsive third-person movement system featuring **double jump**, **dash**, **moving platforms**, and **collectible coins** — establishing a foundation for future gameplay experimentation and polish.

---

## 🎮 Gameplay Overview

Control a nimble character who can **run**, **double jump**, and **dash** through an obstacle-filled 3D environment.  
Master precise movement, time your jumps across **moving platforms**, and collect coins scattered throughout the level.

### 🧩 Core Features
- 🧍 **Custom Character Controller**
  - All movement logic coded from scratch in **C++**, using template animations only for visuals.
- ⚡ **Dash Mechanic**
  - Short burst of directional speed with cooldown and momentum control.
- 🌀 **Double Jump**
  - Secondary mid-air jump tracked and reset on landing.
- 🧱 **Moving Platforms**
  - Sinusoidal platform motion with player velocity inheritance for smooth traversal.
- 💰 **Collectible Coins**
  - Rotating coin actors that detect overlap, increment score, and update UI.
- 🎛️ **UI Integration**
  - On-screen widget showing coin count and (optional) dash cooldown indicator.
- 💀 **Death**
  - Game restarts after 1 second after falling off the platforms

---


