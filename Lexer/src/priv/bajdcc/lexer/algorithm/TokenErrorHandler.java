package priv.bajdcc.lexer.algorithm;

import priv.bajdcc.lexer.error.IErrorHandler;
import priv.bajdcc.lexer.regex.IRegexStringIterator;

/**
 * ������������
 * 
 * @author bajdcc
 *
 */
public abstract class TokenErrorHandler implements IErrorHandler {
	
	/**
	 * �������ӿ�
	 */
	protected IRegexStringIterator m_Iterator = null;

	public TokenErrorHandler(IRegexStringIterator iterator) {
		m_Iterator = iterator;
	}
}
