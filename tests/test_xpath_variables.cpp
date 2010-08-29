#ifndef PUGIXML_NO_XPATH

#include "common.hpp"

#include <string>

TEST(xpath_variables_type_none)
{
	xpath_variable_set set;

	xpath_variable* var = set.add(STR("target"), xpath_type_none);
	CHECK(!var);
}

TEST(xpath_variables_type_boolean)
{
	xpath_variable_set set;

	xpath_variable* var = set.add(STR("target"), xpath_type_boolean);
	CHECK(var);

	CHECK(var->type() == xpath_type_boolean);
	CHECK_STRING(var->name(), STR("target"));

	CHECK(var->get_boolean() == false);
	CHECK_DOUBLE_NAN(var->get_number());
	CHECK_STRING(var->get_string(), STR(""));
	CHECK(var->get_node_set().empty());

	CHECK(var->set(true));
	CHECK(!var->set(1.0));
	CHECK(!var->set(STR("abc")));
	CHECK(!var->set(xpath_node_set()));

	CHECK(var->get_boolean() == true);
	CHECK_DOUBLE_NAN(var->get_number());
	CHECK_STRING(var->get_string(), STR(""));
	CHECK(var->get_node_set().empty());
}

TEST(xpath_variables_type_number)
{
	xpath_variable_set set;

	xpath_variable* var = set.add(STR("target"), xpath_type_number);
	CHECK(var);

	CHECK(var->type() == xpath_type_number);
	CHECK_STRING(var->name(), STR("target"));

	CHECK(var->get_boolean() == false);
	CHECK_DOUBLE(var->get_number(), 0);
	CHECK_STRING(var->get_string(), STR(""));
	CHECK(var->get_node_set().empty());

	CHECK(!var->set(true));
	CHECK(var->set(1.0));
	CHECK(!var->set(STR("abc")));
	CHECK(!var->set(xpath_node_set()));

	CHECK(var->get_boolean() == false);
	CHECK_DOUBLE(var->get_number(), 1);
	CHECK_STRING(var->get_string(), STR(""));
	CHECK(var->get_node_set().empty());
}

TEST(xpath_variables_type_string)
{
	xpath_variable_set set;

	xpath_variable* var = set.add(STR("target"), xpath_type_string);
	CHECK(var);

	CHECK(var->type() == xpath_type_string);
	CHECK_STRING(var->name(), STR("target"));

	CHECK(var->get_boolean() == false);
	CHECK_DOUBLE_NAN(var->get_number());
	CHECK_STRING(var->get_string(), STR(""));
	CHECK(var->get_node_set().empty());

	CHECK(!var->set(true));
	CHECK(!var->set(1.0));
	CHECK(var->set(STR("abc")));
	CHECK(!var->set(xpath_node_set()));

	CHECK(var->get_boolean() == false);
	CHECK_DOUBLE_NAN(var->get_number());
	CHECK_STRING(var->get_string(), STR("abc"));
	CHECK(var->get_node_set().empty());
}

TEST_XML(xpath_variables_type_node_set, "<node/>")
{
	xpath_variable_set set;

	xpath_variable* var = set.add(STR("target"), xpath_type_node_set);
	CHECK(var);

	CHECK(var->type() == xpath_type_node_set);
	CHECK_STRING(var->name(), STR("target"));

	CHECK(var->get_boolean() == false);
	CHECK_DOUBLE_NAN(var->get_number());
	CHECK_STRING(var->get_string(), STR(""));
	CHECK(var->get_node_set().empty());

	CHECK(!var->set(true));
	CHECK(!var->set(1.0));
	CHECK(!var->set(STR("abc")));
	CHECK(var->set(doc.select_nodes(STR("*"))));

	CHECK(var->get_boolean() == false);
	CHECK_DOUBLE_NAN(var->get_number());
	CHECK_STRING(var->get_string(), STR(""));
	CHECK(var->get_node_set().size() == 1 && var->get_node_set()[0] == doc.first_child());
}

TEST(xpath_variables_set_operations)
{
	xpath_variable_set set;

	xpath_variable* v1 = set.add(STR("var1"), xpath_type_number);
	CHECK(v1);

	xpath_variable* v2 = set.add(STR("var2"), xpath_type_string);
	CHECK(v2);

	CHECK(v1 != v2);

	CHECK(set.add(STR("var1"), xpath_type_number) == v1);
	CHECK(set.add(STR("var2"), xpath_type_string) == v2);
	CHECK(set.add(STR("var2"), xpath_type_node_set) == 0);

	CHECK(set.get(STR("var1")) == v1);
	CHECK(set.get(STR("var2")) == v2);
	CHECK(set.get(STR("var")) == 0);
	CHECK(set.get(STR("var11")) == 0);

	CHECK(static_cast<const xpath_variable_set&>(set).get(STR("var1")) == v1);
	CHECK(static_cast<const xpath_variable_set&>(set).get(STR("var3")) == 0);
}

TEST_XML(xpath_variables_set_operations_set, "<node/>")
{
	xpath_variable_set set;

	xpath_variable* v1 = set.add(STR("var1"), xpath_type_number);
	CHECK(v1);

	xpath_variable* v2 = set.add(STR("var2"), xpath_type_string);
	CHECK(v2);

	CHECK(set.set(STR("var1"), 1.0));
	CHECK_DOUBLE(v1->get_number(), 1.0);

	CHECK(set.set(STR("var2"), STR("value")));
	CHECK_STRING(v2->get_string(), STR("value"));

	CHECK(!set.set(STR("var1"), true));

	CHECK(set.set(STR("var3"), doc.select_nodes(STR("*"))));

	xpath_variable* v3 = set.get(STR("var3"));

	CHECK(v3);
	CHECK(v3->type() == xpath_type_node_set);
	CHECK(v3->get_node_set().size() == 1);
}

TEST(xpath_variables_set_out_of_memory)
{
	test_runner::_memory_fail_threshold = 1;

	xpath_variable_set set;

	xpath_variable* var = set.add(STR("target"), xpath_type_number);
	CHECK(!var);
}

TEST(xpath_variables_out_of_memory)
{
	test_runner::_memory_fail_threshold = 64;

	xpath_variable_set set;

	xpath_variable* var = set.add(STR("target"), xpath_type_string);
	CHECK(var);

	CHECK(!var->set(STR("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")));
}

TEST_XML(xpath_variables_evaluate, "<node/>")
{
	xpath_variable_set set;
	set.set(STR("var1"), true);
	set.set(STR("var2"), 0.5);
	set.set(STR("var3"), STR("value"));
	set.set(STR("var4"), doc.select_nodes(STR("*")));

	CHECK_XPATH_BOOLEAN(doc, xpath_query(STR("$var1"), &set), true);
	CHECK_XPATH_NUMBER(doc, xpath_query(STR("$var2"), &set), 0.5);
	CHECK_XPATH_STRING(doc, xpath_query(STR("$var3"), &set), STR("value"));
	CHECK_XPATH_NODESET_Q(doc, xpath_query(STR("$var4"), &set)) % 2;
}

TEST_XML(xpath_variables_evaluate_conversion, "<node>3</node>")
{
	xpath_variable_set set;
	set.set(STR("var"), doc.select_nodes(STR("*")));

	xpath_query query(STR("$var"), &set);

	CHECK_XPATH_BOOLEAN(doc, query, true);
	CHECK_XPATH_NUMBER(doc, query, 3);
	CHECK_XPATH_STRING(doc, query, STR("3"));
	CHECK_XPATH_NODESET_Q(doc, query) % 2;
}

TEST(xpath_variables_evaluate_node_set_fail)
{
	xpath_variable_set set;
	set.set(STR("var"), false);

	xpath_query q(STR("$var"), &set);

#ifdef PUGIXML_NO_EXCEPTIONS
	CHECK_XPATH_NODESET_Q(xml_node(), q);
#else
	try
	{
		q.evaluate_node_set(xml_node());

		CHECK_FORCE_FAIL("Expected exception");
	}
	catch (const xpath_exception&)
	{
	}
#endif
}

TEST_XML(xpath_variables_multiple_documents, "<node/>")
{
	xml_document doc1;
	CHECK(doc1.load(STR("<node/>")));

	xml_document doc2;
	CHECK(doc2.load(STR("<node/>")));

	xpath_variable_set set;
	set.set(STR("var1"), doc1.select_nodes(STR("*")));
	set.set(STR("var2"), doc2.select_nodes(STR("*")));

	xpath_node_set ns = doc.select_nodes(STR("$var1 | $var2 | node"), &set);
	ns.sort();

	CHECK(ns.size() == 3);
	CHECK(ns[0] != ns[1] && ns[0] != ns[2]);
	
	xml_node n0 = doc.child(STR("node")), n1 = doc1.child(STR("node")), n2 = doc2.child(STR("node"));

	CHECK(n0 == ns[0].node() || n0 == ns[1].node() || n0 == ns[2].node());
	CHECK(n1 == ns[0].node() || n1 == ns[1].node() || n1 == ns[2].node());
	CHECK(n2 == ns[0].node() || n2 == ns[1].node() || n2 == ns[2].node());
}

TEST(xpath_variables_long_name)
{
	xpath_variable_set set;
	set.set(STR("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), true);

	CHECK_XPATH_BOOLEAN(xml_node(), xpath_query(STR("$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), &set), true);
}

TEST_XML(xpath_variables_select, "<node attr='1'/><node attr='2'/>")
{
	xpath_variable_set set;
	set.set(STR("one"), 1.0);

	xpath_node_set ns = doc.select_nodes(STR("node[@attr=$one+1]"), &set);
	CHECK(ns.size() == 1 && ns[0].node() == doc.last_child());

	xpath_node n = doc.select_single_node(STR("node[@attr=$one+1]"), &set);
	CHECK(n == ns[0]);
}

TEST(xpath_variables_empty_name)
{
	xpath_variable_set set;
	CHECK(!set.add(STR(""), xpath_type_number));
}

TEST_XML(xpath_variables_inside_filter, "<node key='1' value='2'/><node key='2' value='1'/><node key='1' value='1'/>")
{
	xpath_variable_set set;
	set.set(STR("one"), 1.0);

	xpath_node_set ns = doc.select_nodes(STR("(node[@key = $one])[@value = $one]"), &set);
	CHECK(ns.size() == 1 && ns[0].node() == doc.last_child());
}

TEST_XML(xpath_variables_step, "<node><child/><child/><child><child/></child></node>")
{
	xpath_variable_set set;
	set.set(STR("root"), doc.select_nodes(STR("node")));

	CHECK_XPATH_NODESET_Q(xml_node(), xpath_query(STR("$root/child"), &set)) % 3 % 4 % 5;
	CHECK_XPATH_NODESET_Q(xml_node(), xpath_query(STR("$root//child"), &set)) % 3 % 4 % 5 % 6;
}

TEST_XML(xpath_variables_index, "<node><child/><child/><child><child/></child></node>")
{
	xpath_variable_set set;
	set.set(STR("index"), 2.0);

	CHECK_XPATH_NODESET_Q(doc, xpath_query(STR("node/child[$index]"), &set)) % 4;
	CHECK_XPATH_NODESET_Q(doc, xpath_query(STR("node/child[position()=$index]"), &set)) % 4;
}

TEST(xpath_variables_qname)
{
	xpath_variable_set set;
	set.set(STR("foo:bar"), true);

	CHECK_XPATH_BOOLEAN(xml_node(), xpath_query(STR("$foo:bar"), &set), true);
}

TEST(xpath_variables_name_error)
{
	xpath_variable_set set;
	set.set(STR("foo:"), true);
	set.set(STR(":bar"), true);
	set.set(STR("foo:*"), true);
	set.set(STR("foo"), true);
	set.set(STR("3"), true);

	CHECK_XPATH_FAIL_VAR(STR("$foo:"), &set);
	CHECK_XPATH_FAIL_VAR(STR("$:bar"), &set);
	CHECK_XPATH_FAIL_VAR(STR("$foo:*"), &set);
	CHECK_XPATH_FAIL_VAR(STR("$foo:bar:baz"), &set);
	CHECK_XPATH_FAIL_VAR(STR("$ foo"), &set);

	CHECK_XPATH_FAIL_VAR(STR("$3"), &set);
}

TEST(xpath_variables_empty_string)
{
	xpath_variable_set set;
	set.add(STR("empty"), xpath_type_string);

	CHECK_XPATH_BOOLEAN(xml_node(), xpath_query(STR("$empty = substring-before('a', 'z')"), &set), true);
}

TEST(xpath_variables_name_underscore)
{
	xpath_variable_set set;
	set.set(STR("_foo_bar"), true);

	CHECK_XPATH_BOOLEAN(xml_node(), xpath_query(STR("$_foo_bar"), &set), true);
}

TEST(xpath_variables_name_case)
{
	xpath_variable_set set;
	set.set(STR("i"), 5.0);
	set.set(STR("I"), 2.0);

	CHECK_XPATH_NUMBER(xml_node(), xpath_query(STR("$i div $I"), &set), 2.5);
}

TEST(xpath_variables_name_unicode)
{
#ifdef PUGIXML_WCHAR_MODE
	#ifdef U_LITERALS
		const char_t* name = L"\u0400\u203D";
	#else
		const char_t* name = L"\x0400\x203D";
	#endif
#else
	const char_t* name = "\xd0\x80\xe2\x80\xbd";
#endif

	xpath_variable_set set;
	set.set(name, STR("value"));

	std::basic_string<char_t> var = STR("$");
	var += name;

	CHECK_XPATH_STRING(xml_node(), xpath_query(var.c_str(), &set), STR("value"));
}

TEST_XML(xpath_variables_count_sum, "<node><c1>12</c1><c2>23</c2><c3>34</c3></node>")
{
	xpath_variable_set set;
	set.set(STR("c12"), doc.select_nodes(STR("node/c1 | node/c2")));
	set.set(STR("c3"), doc.select_nodes(STR("node/c3")));
	set.set(STR("c"), doc.select_nodes(STR("node/*")));

	CHECK_XPATH_NUMBER(xml_node(), xpath_query(STR("sum($c12) * count($c) - sum($c3)"), &set), 71);
}
#endif