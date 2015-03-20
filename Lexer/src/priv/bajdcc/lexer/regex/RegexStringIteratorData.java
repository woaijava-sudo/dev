package priv.bajdcc.lexer.regex;

import priv.bajdcc.lexer.token.MetaType;

/**
 * ����ʱʹ�õ�����
 */
public class RegexStringIteratorData {
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