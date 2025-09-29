import optuna_dashboard
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("storage", type=str)
args = parser.parse_args()

print("Starting Optuna dashboard...")
try:
    optuna_dashboard.run_server(
        storage=args.storage,
        host="localhost",
        port=8080
    )
except KeyboardInterrupt:
    print("Dashboard stopped.")
except Exception as e:
    print(f"Error starting dashboard: {e}")