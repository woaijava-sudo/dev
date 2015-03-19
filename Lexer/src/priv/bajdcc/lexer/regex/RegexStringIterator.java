package priv.bajdcc.lexer.regex;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;
import priv.bajdcc.lexer.token.MetaType;

/**
 * �ַ������������ṩ�ֽ�����������
 * 
 * @author bajdcc
 *
 */
public class RegexStringIterator implements IRegexStringIterator {
	/**
	 * �洢�ַ���
	 */
	protected String m_strContext;

	/**
	 * ��ǰ�ķ�����Ϣ
	 */
	protected RegexStringIteratorData m_Data = new RegexStringIteratorData();

	public RegexStringIterator(String strContext) {
		m_strContext = strContext;
	}

	/* ���� Javadoc��
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator#err(priv.bajdcc.lexer.error.RegexException.RegexError)
	 */
	@Override
	public void err(RegexError error) throws RegexException {
		throw new RegexException(error, m_Data.m_iIndex);
	}

	/* ���� Javadoc��
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator#next()
	 */
	@Override
	public void next() {
		if (available()) {
			advance();
		}
		translate();
	}

	/**
	 * ���뵱ǰ�ַ�
	 */
	protected void translate() {
		if (!available()) {
			m_Data.m_chCurrent = 0;
			m_Data.m_kMeta = MetaType.END;
			return;
		}
		m_Data.m_chCurrent = current();
		transform();
	}
	
	/**
	 * �����ַ�����
	 */
	protected void transform() {
		m_Data.m_kMeta = MetaType.CHARACTER;
	}

	/* ���� Javadoc��
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator1#available()
	 */
	@Override
	public boolean available() {
		return m_Data.m_iIndex < m_strContext.length();
	}

	/* ���� Javadoc��
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator1#advance()
	 */
	@Override
	public void advance() {
		m_Data.m_iIndex++;
	}

	/* ���� Javadoc��
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator1#current()
	 */
	@Override
	public char current() {
		return m_strContext.charAt(m_Data.m_iIndex);
	}

	/* ���� Javadoc��
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator1#expect(priv.bajdcc.lexer.token.TokenUtility.MetaType, priv.bajdcc.lexer.error.RegexException.RegexError)
	 */
	@Override
	public void expect(MetaType meta, RegexError error) throws RegexException {
		if (m_Data.m_kMeta == meta) {
			next();
		} else {
			err(error);
		}
	}
}

/**
 * ����ʱʹ�õ�����
 */
class RegexStringIteratorData {
	/**
	 * ��ǰ�����λ��
	 */
	public int m_iIndex = 0;

	/**
	 * �ַ�
	 */
	public char m_chCurrent = 0;

	/**
	 * �ַ�����
	 */
	public MetaType m_kMeta = MetaType.END;

	public RegexStringIteratorData() {

	}

	public RegexStringIteratorData(int index, char current, MetaType meta) {
		m_iIndex = index;
		m_chCurrent = current;
		m_kMeta = meta;
	}
}