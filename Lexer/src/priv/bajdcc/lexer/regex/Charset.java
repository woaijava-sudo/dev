package priv.bajdcc.lexer.regex;

import java.util.ArrayList;

/**
 * �ַ���
 * 
 * @author bajdcc
 */
public class Charset implements IRegexComponent {
	/**
	 * �����ķ�Χ������Χ��
	 */
	public ArrayList<CharacterRange> arrPositiveBounds = new ArrayList<CharacterRange>();

	/**
	 * �Ƿ�ȡ��
	 */
	public boolean bReverse = false;

	/**
	 * ����
	 */
	public enum CharacterType {
		/**
		 * ����ģʽ�������ַ���Χ
		 */
		NORMAL,
		/**
		 * ƥ������
		 */
		BEGIN,
		/**
		 * ƥ����ĩ
		 */
		END
	}

	/**
	 * �ַ������ͣ�ƥ���ַ���ƥ���ף�ƥ��ĩ��
	 */
	public CharacterType kChar = CharacterType.NORMAL;

	@Override
	public void Visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		visitor.visitEnd(this);
	}

	/**
	 * ��ӷ�Χ
	 * 
	 * @param begin
	 *            ����
	 * @param end
	 *            ����
	 */
	public void addRange(char begin, char end) {
		if (begin > end) {
			end = begin;
		}
		arrPositiveBounds.add(new CharacterRange(begin, end));
	}

	/**
	 * ����ַ�
	 * 
	 * @param ch
	 *            �ַ�
	 */
	public void addChar(char ch) {
		arrPositiveBounds.add(new CharacterRange(ch, ch));
	}
}
