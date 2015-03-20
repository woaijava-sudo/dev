package priv.bajdcc.lexer.utility;

/**
 * λ��
 * 
 * @author bajdcc
 *
 */
public class Position {
	/**
	 * �к�
	 */
	public int m_iColumn = 0;

	/**
	 * �к�
	 */
	public int m_iLine = 0;

	public Position() {
	}

	public Position(int col, int line) {
		m_iColumn = col;
		m_iLine = line;
	}

	@Override
	public String toString() {
		return m_iLine + "," + m_iColumn;
	}
}
