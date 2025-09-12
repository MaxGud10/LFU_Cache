import subprocess
from   pathlib import Path

TEST_FILENAME = "ideal_tests.txt"          
PATH_TO_EXECUTABLE = Path("../../build/ideal")  
TIMEOUT_SEC = 5

def read_tests(path: str):
    """Read tests in triples:
       TEST_NUMBER: N
       INPUT: a b c
       OUTPUT: x y
    """
    with open(path, "r", encoding="utf-8") as f:
        lines = [ln.strip() for ln in f if ln.strip()]  

    if len(lines) % 3 != 0:
        raise ValueError("tests file must have a multiple of 3 non-empty lines (TEST_NUMBER/INPUT/OUTPUT).")

    tests = []
    for i in range(0, len(lines), 3):
        k, v = lines[i].split(":", 1)
        if k.strip().upper() != "TEST_NUMBER":
            raise ValueError(f"Expected 'TEST_NUMBER:' on line {i+1}, got: {lines[i]}")
        number = int(v.strip())


        k, v = lines[i+1].split(":", 1)
        if k.strip().upper() != "INPUT":
            raise ValueError(f"Expected 'INPUT:' on line {i+2}, got: {lines[i+1]}")
        input_tokens = v.strip().split()


        k, v = lines[i+2].split(":", 1)
        if k.strip().upper() != "OUTPUT":
            raise ValueError(f"Expected 'OUTPUT:' on line {i+3}, got: {lines[i+2]}")
        ref_tokens = v.strip().split()

        tests.append((number, input_tokens, ref_tokens))
    return tests

def run_tests(tests):
    passed = failed = 0
    exe = str(PATH_TO_EXECUTABLE)

    for number, input_tokens, ref_tokens in tests:
        prog_input = " ".join(input_tokens) + "\n"  
        try:
            res = subprocess.run(
                [exe],
                input=prog_input,
                capture_output=True,
                text=True,
                timeout=TIMEOUT_SEC,
            )
        except FileNotFoundError:
            print(f"Executable not found: {exe}")
            return
        except subprocess.TimeoutExpired:
            print(f"Test {number}: FAILED (timeout)")
            print("Expected:", " ".join(ref_tokens))
            print("Got:     ", "<timeout>")
            failed += 1
            continue

        real_tokens = res.stdout.split()
        ok = (real_tokens == ref_tokens) and (res.returncode == 0)

        if ok:
            passed += 1
        else:
            failed += 1
            print(f"Test {number}: FAILED")
            print("Expected:", " ".join(ref_tokens))
            print("Got:     ", " ".join(real_tokens))
            if res.returncode != 0:
                print(f"(exit code: {res.returncode})")
            if res.stderr.strip():
                print("stderr:", res.stderr.strip())

    print(f"\nPassed tests: {passed}")
    print(f"Failed tests: {failed}")
    print(f"Total: {passed + failed}")

if __name__ == "__main__":
    tests = read_tests(TEST_FILENAME)
    run_tests(tests)