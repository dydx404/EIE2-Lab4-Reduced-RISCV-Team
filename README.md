# EIE2 IAC Lab 4 – Reduced RISC-V CPU  
Repository Owner: **dydx404**

This repository contains our group project for Imperial EIE2 Instruction Architectures & Compilers (IAC) Lab 4. The task is to design and verify a single-cycle Reduced RISC-V CPU capable of running a small instruction subset. All team members must follow the workflow in this document.

---

# 1. Repository Structure

```
src/        – SystemVerilog source files
tb/         – Testbenches (C++ or SV)
sim/        – Simulation outputs (ignored by Git)
docs/       – Documentation (design decisions, challenges, reflection)
README.md   – This file
.gitignore  – Excludes build/simulation artefacts
```

---

# 2. Collaboration Rules

We use a strict feature-branch workflow:

- Do **not** push directly to `main`.
- Each member works only in **their own branch**.
- Integration is done through **Pull Requests (PRs)**.
- Changes must be reviewed before merging.

This prevents conflicts and maintains a stable `main` branch.

---

# 3. Branch Naming

Each member creates a branch according to their assigned module:

| Task                                 | Branch Name Format     |
|-------------------------------------|-------------------------|
| Program Counter + Adders            | `pc-<name>`            |
| Register File + ALU + MUXes         | `rf-alu-<name>`        |
| Control Unit + Sign Extension       | `ctrl-<name>`          |
| Testbench + Verification + Integration | `tb-<name>`         |

Examples:
```
pc-dydx
rf-alu-alice
ctrl-bob
tb-charlie
```

---

# 4. First-Time Setup (Everyone)

## Step 1 — Accept the repository invitation  
Log into GitHub → check notification bell → accept invite for  
`EIE2-Lab4-Reduced-RISCV-Team`.

## Step 2 — Clone the repository  
In your terminal:

```bash
cd ~/Documents
git clone https://github.com/dydx404/EIE2-Lab4-Reduced-RISCV-Team.git
cd EIE2-Lab4-Reduced-RISCV-Team
```

Ensure the folder contains:

```
docs  sim  src  tb  README.md
```

## Step 3 — Create your personal branch  
Replace `<your-branch>`:

```bash
git checkout -b <your-branch>
git push origin <your-branch>
```

Example:
```bash
git checkout -b rf-alu-alice
git push origin rf-alu-alice
```

You will work only on this branch for the entire lab.

---

# 5. Daily Workflow (Every Time You Work)

## Step 1 — Update local main

```bash
git checkout main
git pull origin main
```

## Step 2 — Sync your branch with main

```bash
git checkout <your-branch>
git merge main
```

Resolve conflicts if prompted.

---

# 6. Making Changes

After editing files:

### See what changed:
```bash
git status
```

### Stage files:
```bash
git add <files>
```
or:
```bash
git add .
```

### Commit:
```bash
git commit -m "Description of work done"
```

### Push:
```bash
git push origin <your-branch>
```

Simulation artefacts (`obj_dir/`, `*.vcd`, `*.log`) are ignored automatically.

---

# 7. Submitting Your Work (Pull Request Workflow)

When your module is functional and tested:

1. Push latest updates:
   ```bash
   git push origin <your-branch>
   ```

2. On GitHub, open the repository.

3. Click **Compare & pull request**  
   or go to **Pull Requests → New pull request**.

4. Select:
   - Base: `main`
   - Compare: `<your-branch>`

5. In the PR description include:
   - What you implemented  
   - How you tested it  
   - Any known issues or TODOs  

6. Request review from teammates.

7. Merge only after approval.

---

# 8. After Your Pull Request Is Merged

Update your local repo:

```bash
git checkout main
git pull origin main
git checkout <your-branch>
git merge main
```

Or create a fresh branch if starting a new task:

```bash
git checkout main
git checkout -b <new-branch>
git push origin <new-branch>
```

---

# 9. Quick Reference Cheat Sheet

```
git checkout main
git pull origin main
git checkout <your-branch>
git merge main

git status
git add .
git commit -m "message"
git push origin <your-branch>

# Open PR on GitHub → wait for review → merge.
```

---

All members must follow this workflow to ensure a clean, stable project history and smooth collaboration.
