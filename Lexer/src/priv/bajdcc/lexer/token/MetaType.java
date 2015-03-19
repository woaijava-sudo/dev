package priv.bajdcc.lexer.token;

/**
 * ×Ö·ûÀàÐÍ
 */
public enum MetaType {
	CHARACTER('\0'), LPARAN('('), RPARAN(')'), CARET('^'), DOLLAR('$'), STAR(
			'*'), PLUS('+'), QUERY('?'), LSQUARE('['), RSQUARE(']'), BAR('|'), ESCAPE(
			'\\'), DASH('-'), LBRACE('{'), RBRACE('}'), COMMA(','), NEW_LINE(
			'\n'), CARRIAGE_RETURN('\r'), BACKSPACE('\b'), END('\0');

	private char ch;

	MetaType(char ch) {
		this.ch = ch;
	}

	public char getChar() {
		return ch;
	}

	public void setChar(char ch) {
		this.ch = ch;
	}
}
