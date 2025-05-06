# Session Title / Description

## :dart: Goals
- Goal 1: Learn about Regularization in PWA
- Goal 2: Connect to Maximum likelihood, Markov Chain Monte Carlo, and Information Field Theory
- Goal 3: Learn about a streamlined framework to do this

```bash
# --bind whatever directories you need access to
apptainer exec --contain --writable-tmpfs \
    --bind /my/working/directory \
    --bind /scratch \
    --bind ~/.cache/fontconfig \
    --env BASH_ENV=/dev/null \
    /w/halld-scshelf2101/lng/WORK/PyAmpTools/pyamptools.sif bash

# INSIDE THE CONTAINER:
source /etc/bash.bashrc
```

## :clipboard: Prerequisites
- General understanding of PWA, ability to connect to the jlab farm
- `analysis_chain.py` is a tutorial as of this writing
  - `analysis_chain.ipynb` is available if you can get it work following below instructions
  - see **Analysis Tutorial** below for updated version
- Useful links:
  - [Analysis Tutorial](https://lan13005.github.io/PyAmpTools/demos/analysis_chain.html)
    - jupyter through ssh
      - [vscode:](https://lan13005.github.io/PyAmpTools/intro.html#for-vscode-remote-containter-development:~:text=For%20VSCode%20Remote%20Containter%20Development)
      - [terminal:](https://lan13005.github.io/PyAmpTools/intro.html#for-vscode-remote-containter-development:~:text=Additional%20Information-,Jupyter%20notebooks%20in%20VSCode,-%23)
  - [Regularization Documentation](https://halldweb.jlab.org/doc-private/DocDB/ShowDocument?docid=6826)

## :file_folder: Materials Provided
- Nothing, just connect to the container on the farm, everything is self contained as it is an input output test -> drawing simulations then fitting

## :memo: Session Outline
1. **Introduction**: Presentation on Regularization and the pyamptools framework
2. **Wrap-Up**: Run the tutorial