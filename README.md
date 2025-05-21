# Canvas_Quantum_Simulator

Canvas Quantum Simulator – v2.0 "Spaceship edition" 🚀
-------------------------------------------------------------
## Features
 • A quantum circuit simulator using scene graph architecture
 • Support for quantum gates via JSON model files
 • ASCII visualization of quantum circuits
 • Integration with Eigen for matrix operations
 • Uses nlohmann/json for JSON handling

## Latest features
 • Unlimited "models" (unitaries, channels, meta‑nodes)
 • SceneGraph with Group, Repeat (scale), ParamBind (rotate/param)
 • JSON plug‑in format extended with arbitrary properties
 • Compiler flattens graph → gate list for state‑vector engine
 • ASCII canvas now draws nested groups + repeat loops
-------------------------------------------------------------
Build with MSVC using [Github actions](https://github.com/Andycar/Canvas_Quantum_Simulator/actions)

> Dependencies: [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page), [nlohmann/json](https://github.com/nlohmann/json)
