# Welcome!
This folder contains all the code, talks, and info from the [2025 GlueX Software Tutorial](https://halldweb.jlab.org/wiki/index.php/GlueX_Tutorial_2025#Agenda). The goals of this tutorial are to:
1. :mag: Provide an overview of the methods to search for exotic mesons
2. :bar_chart: Help students and new collaborators get started with the technical steps necessary for a physics analysis

To demonstrate this, we'll be using a fictitious analysis of the reaction $\gamma p \rightarrow b_1(1235)p \rightarrow \omega(782)\pi^0 p$. The sub-folders here are divided into their session number, with the following topics:

| Session/Topic                          | Details                                        | Presenter(s)      |
|:---------------------------------------|:-----------------------------------------------|:------------------|
| **Introduction & Overview of Methods** |                                                |                   |
| Session 1a                             | *Scientific Computing at JLab*                 | Alex Austregesilo |
| Session 1b                             | *Event Reconstruction in GlueX*                | Simon Taylor      |
| **Computing & Tools**                  |                                                |                   |
| Session 1c                             | *Introduction to Amplitude Analysis*           | Boris Grube       |
| Session 1d                             | *GlueX MC Generation*                          | Peter Hurck       |
| **MC Generation and Event Selection**  |                                                |                   |
| Session 2a                             | *MC Signal Generator gen_amp_V2*               | Kevin Saldana     |
| Session 2b                             | *MC Background Generator bggen_upd*            | Eugene Chudakov   |
| Session 2c                             | *Event Selection with DSelector*               | Edmundo Barriga   |
| Session 2c                             | *Event Selection with FSRoot*                  | Amy Schertz       |
| **Advanced Methods**                   |                                                |                   |
| Session 2d                             | *Event Selection with AI/ML*                   | Igal Jaegle       |
| Session 2e                             | *AmpTools: Fitting and Plotting with python*   | Kevin Scheuer     |
| Session 2f                             | *Beyond AmpTools*                              | Lawrence Ng       |

## 🛠️ Setup & Environment

Before diving into the sessions, ensure your environment is properly set up. Follow these steps:

1. Login to an ifarm node:
    - [Details can be found here](https://jlab.servicenowservices.com/kb?id=kb_article_view&sysparm_article=KB0015066)
    - Registering for a JLab account, JLab computing resources, and general getting started info [can be found here](https://halldweb.jlab.org/wiki/index.php/Getting_Started_At_GlueX)

2. Clone the tutorial repository:    
    - It is strongly recommended to do this on the work disk at `/w/halld-scshelf2101/home/$USER/`
    ```bash
    git clone https://github.com/JeffersonLab/gluex_workshops.git
    cd tutorial_2025
    ```

3. Set up your environment variables (adjust based on your shell):
    - For `tcsh` shell (ifarm default):
      ```bash
      source setup_gluex.csh
      ```
    - For `sh` or `bash` shell:
      ```bash
      source setup_gluex.sh
      ```

4. Verify your setup:
    ```bash
    bash verify_setup.sh
    ```

If you encounter any issues, contact the session presenters.