#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/n0kz_attributes.h>

struct n0kz_attributes n0kz_data = {
    .n0kz_kgsl_skip_zeroing = 0,
};

#define N0KZ_ATTR_RW(name) \
static ssize_t name##_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) \
{ \
    return sprintf(buf, "%d\n", n0kz_data.name); \
} \
static ssize_t name##_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) \
{ \
    int val; \
    if (kstrtoint(buf, 10, &val)) \
        return -EINVAL; \
    n0kz_data.name = val; \
    return count; \
} \
static struct kobj_attribute name##_attr = __ATTR(name, 0644, name##_show, name##_store);

N0KZ_ATTR_RW(n0kz_kgsl_skip_zeroing);

static struct attribute *n0kz_attrs[] = {
    &n0kz_kgsl_skip_zeroing_attr.attr,
    NULL
};

static struct attribute_group n0kz_attr_group = {
    .attrs = n0kz_attrs,
};

static struct kobject *n0kz_kobj;

static int __init n0kz_attributes_init(void) {
    int retval;

    n0kz_kobj = kobject_create_and_add("n0kz_attributes", kernel_kobj);
    if (!n0kz_kobj)
        return -ENOMEM;

    retval = sysfs_create_group(n0kz_kobj, &n0kz_attr_group);
    if (retval)
        kobject_put(n0kz_kobj);

    return retval;
}

static void __exit n0kz_attributes_exit(void) {
    kobject_put(n0kz_kobj);
}

module_init(n0kz_attributes_init);
module_exit(n0kz_attributes_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kvsnr113 <kvsnrprojkt113@gmail.com>");
MODULE_DESCRIPTION("N0KZ kgsl skip zeroing attribute module");
