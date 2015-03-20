package priv.bajdcc.lexer.token;

import priv.bajdcc.lexer.utility.Position;

/**
 * ����
 * 
 * @author bajdcc
 */
public class Token {
	/**
	 * ��������
	 */
	public TokenType m_kToken = TokenType.ERROR;

	/**
	 * ����
	 */
	public Object m_Object = null;

	/**
	 * λ��
	 */
	public Position m_Position = new Position();

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(String.format("%04d,%03d:\t%-10s\t%s", m_Position.m_iLine, m_Position.m_iColumn,
				m_kToken.getName(),
				m_Object == null ? "(null)" : m_Object.toString()));
		return sb.toString();
	}
}