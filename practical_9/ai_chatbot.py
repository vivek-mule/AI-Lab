from __future__ import annotations

import os
from pathlib import Path
from typing import Any

try:
	import google.generativeai as genai
	from google.api_core import exceptions as genai_exceptions
except Exception as import_error:
	genai = None
	genai_exceptions = None
	_IMPORT_ERROR = import_error
else:
	_IMPORT_ERROR = None


DEFAULT_CONTEXT_FILE = Path(__file__).with_name("description.txt")
MODEL_NAME = "models/gemini-2.5-flash"
FALLBACK_REPLY = "I'm sorry, I don't have that information right now."
EXIT_COMMANDS = {"quit", "exit", "bye"}
API_KEY = "api_key_here"  # Replace with your Gemini API key


def read_context(context_path: Path) -> str:
	try:
		raw_text = context_path.read_text(encoding="utf-8")
	except FileNotFoundError as exc:
		raise SystemExit(
			f"Context file not found at '{context_path}'. Please create it with your organization details."
		) from exc

	normalized = raw_text.strip()
	if not normalized:
		raise SystemExit(
			f"Context file '{context_path}' is empty. Populate it with relevant information before chatting."
		)

	return normalized


def build_prompt(context: str, question: str) -> str:

	return (
		"You are a helpful assistant for an organization."
		" Answer ONLY using the facts contained inside the <knowledge_base> tags."
		" If the answer is not present, respond exactly with '"
		f"{FALLBACK_REPLY}' and nothing else.\n\n"
		"<knowledge_base>\n"
		f"{context}\n"
		"</knowledge_base>\n\n"
		"User question: "
		f"{question}\n\n"
		"Assistant response:"
	)


def get_response(model: Any, prompt: str) -> str:

	try:
		result = model.generate_content(prompt, request_options={"timeout": 30})
	except genai_exceptions.GoogleAPIError as api_error:
		raise SystemExit(f"Gemini API call failed: {api_error}") from api_error

	text = (result.text or "").strip()
	return text or FALLBACK_REPLY


def main() -> None:
	if genai is None or genai_exceptions is None:
		raise SystemExit(
			"The google-generativeai package is required. Install it with:\n"
			"    pip install google-generativeai"
		) from _IMPORT_ERROR

	api_key = API_KEY.strip()
	if not api_key or api_key == "YOUR_GEMINI_API_KEY":
		raise SystemExit(
			"Update the API_KEY constant in ai_chatbot.py with your Gemini API key before running this script."
		)

	genai.configure(api_key=api_key)

	context_file = Path(os.getenv("CHATBOT_CONTEXT_FILE", DEFAULT_CONTEXT_FILE))
	context_text = read_context(context_file)

	model_name = os.getenv("CHATBOT_MODEL_NAME", MODEL_NAME).strip()
	model = genai.GenerativeModel(model_name)
	print(f"Using Gemini model: {model_name}")

	print("AI Chatbot ready. Ask a question related to the organization context."
		  " Type 'quit', 'exit', or 'bye' to leave.\n")

	while True:
		try:
			user_input = input("You: ")
		except (EOFError, KeyboardInterrupt):
			print("\nAssistant: Goodbye!")
			break

		cleaned = user_input.strip()
		if not cleaned:
			print("Assistant: Please enter a question.")
			continue

		if cleaned.lower() in EXIT_COMMANDS:
			print("Assistant: Goodbye!")
			break

		prompt = build_prompt(context_text, cleaned)
		reply = get_response(model, prompt)
		print(f"Assistant: {reply}")


if __name__ == "__main__":
	main()
