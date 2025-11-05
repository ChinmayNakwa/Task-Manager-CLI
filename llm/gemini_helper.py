import os
import sys
import google.generativeai as genai
from dotenv import load_dotenv

def main():
    """
    This script takes a high-level goal and generates the specific `taskmaster`
    CLI commands required to create a project plan.
    """
    try:
        # Load environment variables from .env file
        load_dotenv()
        
        if not os.getenv("GEMINI_API_KEY"):
            print("Error: GEMINI_API_KEY environment variable not set.", file=sys.stderr)
            sys.exit(1)

        genai.configure(api_key=os.getenv("GEMINI_API_KEY"))
        client = genai.GenerativeModel("gemini-2.5-flash")
        goal_description = sys.stdin.read().strip()
        if not goal_description:
            print("Error: No goal description provided via stdin.", file=sys.stderr)
            sys.exit(1)

        prompt = f"""You are an expert project manager. Your task is to take a high-level goal and break it down into the exact `taskmaster` CLI commands needed to create the project plan.

        The available commands are:
        - `add "<description>"`
        - `add "<description>" --priority <1-3>`
        - `depends <task_to_wait_for_id> <task_that_waits_id>`

        RULES:
        1. Generate a sequence of `add` commands first for all tasks.
        2. Then, generate the `depends` commands.
        3. For dependencies, refer to tasks by their order of creation. Use placeholders like `<ID_1>`, `<ID_2>`, etc., where `<ID_1>` refers to the first task you added.
        4. Infer logical dependencies (e.g., 'writing' comes before 'editing').
        5. Infer task priorities where applicable (e.g., 'planning' is high priority).
        6. Output ONLY the commands, each on a new line. Do not add any other text.

        EXAMPLE:
        GOAL: "Create a simple web app"
        COMMANDS:
        add "Design the UI/UX" --priority 3
        add "Develop the frontend"
        add "Develop the backend API"
        add "Deploy the application"
        depends <ID_1> <ID_2>
        depends <ID_3> <ID_2>
        depends <ID_2> <ID_4>
        
        ---
        
        GOAL: "{goal_description}"
        COMMANDS:"""

        model = genai.GenerativeModel('gemini-2.5-flash')
        response = model.generate_content(prompt)
        
        # The rest of the script is the same: print the raw output.
        print(response.text.strip())

    except Exception as e:
        print(f"An error occurred in the Python script: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()