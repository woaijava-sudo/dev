package priv.bajdcc.lexer.regex;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;

/**
 * 
 * �����������
 * 
 * @author bajdcc
 *
 */
public class RegexStringUtility {

	/**
	 * �����ӿ�
	 */
	private IRegexStringIterator m_Iterator = null;

	public RegexStringUtility(IRegexStringIterator iterator) {
		m_Iterator = iterator;
	}

	/**
	 * ����ת���ַ�
	 * 
	 * @param ch
	 *            �ַ�
	 * @return �������ַ�
	 * @throws RegexException 
	 */
	public char fromEscape(char ch, RegexError error) throws RegexException {
		if (ch == 'r') {
			ch = '\r';
		} else if (ch == 'n') {
			ch = '\n';
		} else if (ch == 't') {
			ch = '\t';
		} else if (ch == 'b') {
			ch = '\b';
		} else if (ch == 'f') {
			ch = '\f';
		} else if (ch == 'x') {
			ch = fromDigit(16, 2, error);
		} else if (ch == 'o') {
			ch = fromDigit(8, 3, error);
		} else if (ch == 'u') {
			ch = fromDigit(16, 4, error);
		} else {
			m_Iterator.err(RegexError.ESCAPE);
		}
		return ch;
	}

	/**
	 * ��������
	 * 
	 * @param base
	 *            ����
	 * @param count
	 *            ����
	 * @return �������ַ�
	 * @throws RegexException
	 */
	public char fromDigit(int base, int count, RegexError error)
			throws RegexException {
		int chv, val = 0;
		try {
			while (count != 0) {
				chv = Integer.valueOf(m_Iterator.current() + "", base);
				--count;
				val *= base;
				val += chv;
				m_Iterator.next();
			}
		} catch (NumberFormatException e) {
			m_Iterator.err(error);
		}
		return (char) val;
	}
}
