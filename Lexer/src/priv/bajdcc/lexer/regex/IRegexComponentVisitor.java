package priv.bajdcc.lexer.regex;

/**
 * ����������ʽ����ķ��ʽӿڣ�Visitorģʽ��
 * 
 * @author bajdcc
 */
public interface IRegexComponentVisitor {

	public void visitBegin(Charset node);

	public void visitBegin(Constructure node);

	public void visitBegin(Repetition node);

	public void visitEnd(Charset node);

	public void visitEnd(Constructure node);

	public void visitEnd(Repetition node);
}
