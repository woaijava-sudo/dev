package priv.bajdcc.lexer.algorithm;

import priv.bajdcc.lexer.regex.IRegexStringIterator;

/**
 * ��ǰ��һ���Ĵ�������
 * 
 * @author bajdcc
 *
 */
public class TokenErrorAdvanceHandler extends TokenErrorHandler {

	public TokenErrorAdvanceHandler(IRegexStringIterator iterator) {
		super(iterator);
	}

	@Override
	public void handleError() {
		m_Iterator.advance();
	}
}