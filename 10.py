import re
import sys

import pulp


# ---------- Parsing ----------

def parse_line(line):
    """
    Parse one puzzle line of the form:
      [ignoredstuff] (a,b,...) (c,d,...) ... {t0,t1,...}
    Returns (buttons, target) or None if the line is empty/invalid.

    buttons: list[list[int]]
    target:  list[int]
    """
    line = line.strip()
    if not line or line.startswith("#"):
        return None

    # Drop leading [...] if present
    if ']' in line:
        line = line.split(']', 1)[1].strip()

    # Find target {...}
    target_match = re.search(r'\{(.*?)\}', line)
    if not target_match:
        return None

    target_str = target_match.group(1)
    before_brace = line.split('{', 1)[0]

    # Extract button patterns: (...) from before_brace
    button_parts = re.findall(r'\((.*?)\)', before_brace)

    buttons = []
    for part in button_parts:
        part = part.strip()
        if not part:
            buttons.append([])
        else:
            indices = [int(x.strip()) for x in part.split(',') if x.strip() != ""]
            buttons.append(indices)

    target = [int(x.strip()) for x in target_str.split(',') if x.strip() != ""]

    if not buttons or not target:
        return None
    return buttons, target


# ---------- Build matrix ----------

def build_matrix(buttons, target):
    """
    Build A (rows = indices, cols = buttons) and b (plain ints).
    A[i][j] = 1 if button j affects index i, else 0.
    """
    num_indices = len(target)
    num_buttons = len(buttons)

    A = [[0 for _ in range(num_buttons)] for _ in range(num_indices)]
    for j, btn in enumerate(buttons):
        for idx in btn:
            if idx < 0 or idx >= num_indices:
                raise ValueError(
                    f"Button {j} uses index {idx}, "
                    f"but target length is {num_indices}"
                )
            A[idx][j] = 1

    b = list(target)
    return A, b


# ---------- ILP solving ----------

def solve_via_ilp(A, b, puzzle_index=None):
    """
    Solve A x = b, x >= 0 integer, minimizing sum(x).
    Uses PuLP. Returns (solution_list, status_string) or (None, status_string).
    """
    num_indices = len(A)
    num_buttons = len(A[0]) if A else 0

    prob_name = f"puzzle_{puzzle_index or 0}"
    prob = pulp.LpProblem(prob_name, pulp.LpMinimize)

    # Decision variables
    x_vars = [
        pulp.LpVariable(f"x_{j}", lowBound=0, cat=pulp.LpInteger)
        for j in range(num_buttons)
    ]

    # Objective: minimize total button clicks
    prob += pulp.lpSum(x_vars)

    # Constraints: A x = b
    for i in range(num_indices):
        prob += (
            pulp.lpSum(A[i][j] * x_vars[j] for j in range(num_buttons)) == b[i],
            f"row_{i}",
        )

    # Solve
    status = prob.solve(pulp.PULP_CBC_CMD(msg=False))
    status_str = pulp.LpStatus[status]

    if status_str != "Optimal":
        return None, status_str

    sol = [int(pulp.value(xj)) for xj in x_vars]
    return sol, status_str


# ---------- Main driver ----------

def main(input_path):
    totals = []
    statuses = []

    with open(input_path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    line_no = 0
    for raw_line in lines:
        line_no += 1
        parsed = parse_line(raw_line)
        if parsed is None:
            continue  # skip empty/invalid lines

        buttons, target = parsed

        try:
            A, b = build_matrix(buttons, target)
        except ValueError as e:
            print(f"# Line {line_no}: ERROR building matrix: {e}", file=sys.stderr)
            continue

        sol, status = solve_via_ilp(A, b, puzzle_index=line_no)
        statuses.append(status)

        if sol is None:
            print(f"# Line {line_no}: ILP status = {status}, no solution", file=sys.stderr)
            continue

        total = sum(sol)
        totals.append(total)
        # Print one number per puzzle (as requested)
        print(total)

    # Summary
    if totals:
        overall_sum = sum(totals)
        mn = min(totals)
        mx = max(totals)
        avg = overall_sum / len(totals)
        optimal_count = sum(1 for s in statuses if s == "Optimal")

        print("\n--- Summary ---", file=sys.stderr)
        print(f"Number of solved puzzles: {len(totals)}", file=sys.stderr)
        print(f"Total button presses over all solved puzzles: {overall_sum}", file=sys.stderr)
        print(f"Min presses in a puzzle: {mn}", file=sys.stderr)
        print(f"Max presses in a puzzle: {mx}", file=sys.stderr)
        print(f"Average presses per puzzle: {avg:.2f}", file=sys.stderr)
        print(f"Puzzles with ILP status 'Optimal': {optimal_count}", file=sys.stderr)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python solve_puzzles_all_ilp.py i10.txt")
    else:
        main(sys.argv[1])
