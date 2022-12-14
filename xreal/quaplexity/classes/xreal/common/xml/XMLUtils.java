package xreal.common.xml;

import java.io.*;
import java.lang.reflect.*;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.*;


import xreal.Engine;

/**
 * Class for accessing basic XML/DOM methods.
 */
public class XMLUtils {
	

	// hints for how we want documents written
	public final static int OUTPUT_PRETTY = 1;
	public final static int OUTPUT_COMPACT = 2;

	/**
	 * Copy the source node into (inside) the destination element - the two may
	 * (should) be in different documents (if they were in the same document -
	 * there are much easier ways to do this).
	 * 
	 * @param source
	 *            org.w3c.dom.Element
	 * @param dest
	 *            org.w3c.dom.Element
	 * @param skipRootElement
	 *            - if the source is an Element, we may skip copying the source
	 *            element itself, and only copy its children into the source. If
	 *            the source is a Document, we may choose to skip the document's
	 *            root element.
	 */
	public static void copy(Node source, Element dest, boolean skipSourceRootElement) {
		switch (source.getNodeType()) {
		case Node.ELEMENT_NODE:
			Element newElement;
			if (skipSourceRootElement)
				newElement = dest;
			else {
				newElement = ((Document) dest.getOwnerDocument()).createElement(((Element) source).getTagName());

				// copy attributes
				NamedNodeMap nnm = source.getAttributes();
				for (int i = 0; i < nnm.getLength(); i++) {
					Node n = nnm.item(i);
					newElement.setAttribute(n.getNodeName(), n.getNodeValue());
				}

				// put new element under the destination
				dest.appendChild(newElement);
			}

			// copy children
			NodeList nl = source.getChildNodes();
			for (int i = 0; i < nl.getLength(); i++) {
				Node n = nl.item(i);
				copy(n, newElement, false);
			}
			break;

		case Node.TEXT_NODE:
			Text newText = ((Document) dest.getOwnerDocument()).createTextNode(source.getNodeValue());
			dest.appendChild(newText);
			break;

		case Node.CDATA_SECTION_NODE:
			CDATASection newCData = ((Document) dest.getOwnerDocument()).createCDATASection(source.getNodeValue());
			dest.appendChild(newCData);
			break;

		case Node.PROCESSING_INSTRUCTION_NODE:
			ProcessingInstruction newPI = ((Document) dest.getOwnerDocument()).createProcessingInstruction(((ProcessingInstruction) source).getTarget(),
					((ProcessingInstruction) source).getData());
			dest.appendChild(newPI);
			break;

		case Node.COMMENT_NODE:
			Comment newComment = ((Document) dest.getOwnerDocument()).createComment(source.getNodeValue());
			dest.appendChild(newComment);
			break;

		case Node.DOCUMENT_NODE:
			copy(((Document) source).getDocumentElement(), dest, skipSourceRootElement);
			break;
		}
	}

	/**
	 * Create a blank DOM XML document.
	 * 
	 * @return org.w3c.dom.Document
	 */
	public static Document createXMLDocument() {
		Document doc = null;

		try {
			// Create instance of DocumentBuilderFactory
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			
			// Get the DocumentBuilder
			DocumentBuilder parser = factory.newDocumentBuilder();
			
			// Create blank DOM Document
			doc = parser.newDocument();
		} catch (Exception e) {
			System.out.println(e.getMessage());
		}

		return doc;
	}
	
	/**
	 * Read an XML file into a DOM document.
	 */
	public static Document readXMLDocument(String fileName)
			throws IOException {

		Document doc = null;

		try {
			// Create instance of DocumentBuilderFactory
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			
			// Get the DocumentBuilder
			DocumentBuilder parser = factory.newDocumentBuilder();
			
			byte byteArray[] = Engine.readFile(fileName);
			if (byteArray == null) {
				throw new RuntimeException("Could not load '" + fileName + "'");
			}

			// Engine.println("byteArray length = " + byteArray.length);

			// reader = new BufferedReader(new InputStreamReader(new
			// ByteArrayInputStream(byteArray)));
			ByteArrayInputStream reader = new ByteArrayInputStream(byteArray);
			
			// Create blank DOM Document
			doc = parser.parse(reader);
			
		} catch (Exception e) {
			e.getMessage();
		}

		return doc;

		// XMLParser p = new XMLParser(r, sourceName);
		// return p.parseDocument();
	}

	/**
	 * write a DOM document to an XML stream.
	 * @throws Throwable 
	 * @throws TransformerConfigurationException 
	 */
	public static void writeXMLDocument(Document doc, String fileName) throws IOException, TransformerConfigurationException, Throwable {
		
		Transformer transformer = TransformerFactory.newInstance().newTransformer();
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		//transformer.setOutputProperty(OutputKeys.)
		
		// initialize StreamResult with File object to save to file
		StreamResult result = new StreamResult(new ByteArrayOutputStream());
		DOMSource source = new DOMSource(doc);
		transformer.transform(source, result);
		
		ByteArrayOutputStream writer = (ByteArrayOutputStream) result.getOutputStream();

		//String xmlString = result.getWriter().toString();
		//System.out.println(xmlString);
		
		//System.out.println(writer.toString());
		
		Engine.writeFile(fileName, writer.toByteArray());
		
		
		/*
		StreamFormat openXMLStyle = StreamFormat.XML_PRETTY;

		// map XMLTools styles to OpenXML Styles
		switch (outputStyle) {
		case XMLTools.OUTPUT_PRETTY:
			openXMLStyle = StreamFormat.XML_PRETTY;
			break;

		case XMLTools.OUTPUT_COMPACT:
			openXMLStyle = StreamFormat.XML_COMPACT;
			break;
		}

		Publisher pub = PublisherFactory.createPublisher(w, openXMLStyle);
		pub.publish(doc);
		pub.close();
		*/
	}

	/**
	 * Look for &lt;param name="xxx" value="yyy"&gt; tags under a given element,
	 * and if any are found, try calling setXxx(yyy) on the given object.
	 * 
	 * @param e
	 *            a DOM element possibly containing &lt;param&gt; tags.
	 * @param obj
	 *            an Object possibly containing methods with the signature
	 *            setXxx(String s).
	 */
	public static void parseParams(Element e, Object obj) {
		parseParams(e, obj, null);
	}

	/**
	 * Look for &lt;param name="xxx" value="yyy"&gt; tags under a given element,
	 * and if any are found, try calling setXxx(yyy) on the given object.
	 * 
	 * @param e
	 *            a DOM element possibly containing &lt;param&gt; tags.
	 * @param obj
	 *            an Object possibly containing methods with the signature
	 *            setXxx(String s).
	 * @param targetClass
	 *            the class we want to treat obj as, that way a superclass can
	 *            call this method and limit it to only setting parameters found
	 *            in the superclass, and not in any subclasses (useful if you're
	 *            calling this from a constructor, and the subclass constructors
	 *            and instance initializers haven't been invoked yet) - may be
	 *            null if you don't care.
	 */
	public static void parseParams(Element e, Object obj, Class targetClass) {
		// declare some variable, but don't actually create them
		// now, since they may not be needed.
		Class cls = null;
		Class[] methodParamTypes = null;
		Object[] methodParams = null;

		for (Node n = e.getFirstChild(); n != null; n = n.getNextSibling()) {
			if (n.getNodeType() != Node.ELEMENT_NODE)
				continue;

			Element e2 = (Element) n;
			if (!"param".equals(e2.getTagName()))
				continue;

			String propertyName = e2.getAttribute("name");
			if (propertyName == null)
				continue;

			String propertyValue = e2.getAttribute("value");
			if (propertyValue == null)
				continue;

			String gameletSetter = "set" + Character.toUpperCase(propertyName.charAt(0)) + propertyName.substring(1);

			try {
				// ok, we need those variables, so go
				// ahead and create them
				if (cls == null) {
					cls = obj.getClass();
					methodParamTypes = new Class[1];
					methodParamTypes[0] = String.class;
					methodParams = new Object[1];
				}

				Method setter;
				if (targetClass == null)
					setter = cls.getMethod(gameletSetter, methodParamTypes);
				else
					setter = targetClass.getDeclaredMethod(gameletSetter, methodParamTypes);

				methodParams[0] = propertyValue;
				setter.invoke(obj, methodParams);
			} catch (NoSuchMethodException nsme) {
				// Game.dprint(gameletSetter + " method not found\n");
			} catch (InvocationTargetException ite) {
				ite.getTargetException().printStackTrace();
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
	}
}