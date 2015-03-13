package priv.bajdcc.lexer.regex;

/**
 * ѭ������
 * 
 * @author bajdcc
 */
public class Repetition implements IRegexComponent {
	/**
	 * ѭ���������ʽ
	 */
	public IRegexComponent Component;
	/**
	 * ѭ����������
	 */
	public int iBegin = 0;
	/**
	 * ѭ����������
	 */
	public int iEnd = 0;
	/**
	 * �Ƿ�̰��ģʽ
	 */
	public boolean bGreed = false;

	@Override
	public void Visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		Component.Visit(visitor);
		visitor.visitEnd(this);
	}
}
