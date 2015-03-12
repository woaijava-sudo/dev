package cn.njnu.edu.bajdcc.lexer.regex;

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

	@Override
	public void Visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		visitor.visitEnd(this);		
	}
}
