
from difflib import SequenceMatcher
from typing import List, Tuple

KNOWLEDGE_BASE: List[Tuple[str, str]] = [
    (
        "what are your support hours",
        "Our support team is available Monday through Friday from 9 AM to 6 PM",
    ),
    (
        "how can i reset my password",
        "Visit the account settings page and click 'Forgot Password'. We'll email you reset instructions.",
    ),
    (
        "where are you located",
        "We're a fully remote team with teammates across the globe!",
    ),
    (
        "do you offer refunds",
        "Yes. Contact support within 30 days of purchase so we can help you with a refund.",
    ),
    (
        "how do i contact support",
        "Email support@example.com or start a chat from the help center.",
    ),
]

LOW_CONFIDENCE_RESPONSE = "Sorry, I don't have an answer for that yet."
EXIT_COMMANDS = {"quit", "exit", "bye"}
SIMILARITY_THRESHOLD = 0.6


def _normalize(text: str) -> str:

    return " ".join(text.strip().lower().split())


def find_best_match(user_query: str) -> Tuple[float, str]:

    normalized_query = _normalize(user_query)
    best_score = 0.0
    best_answer = ""

    for question, answer in KNOWLEDGE_BASE:
        score = SequenceMatcher(None, normalized_query, _normalize(question)).ratio()
        if score > best_score:
            best_score = score
            best_answer = answer

    return best_score, best_answer


def main() -> None:
    print("Simple FAQ Chatbot. Type 'quit', 'exit', or 'bye' to leave.\n")

    while True:
        try:
            user_input = input("You: ")
        except (EOFError, KeyboardInterrupt):
            print("\nAssistant: Goodbye!")
            break

        cleaned_input = user_input.strip()
        if not cleaned_input:
            print("Assistant: Please enter a question.")
            continue

        if cleaned_input.lower() in EXIT_COMMANDS:
            print("Assistant: Goodbye!")
            break

        score, answer = find_best_match(cleaned_input)
        if score >= SIMILARITY_THRESHOLD:
            print(f"Assistant: {answer}")
        else:
            print(f"Assistant: {LOW_CONFIDENCE_RESPONSE}")


if __name__ == "__main__":
    main()
