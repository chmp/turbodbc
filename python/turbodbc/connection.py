from __future__ import absolute_import

from .exceptions import translate_exceptions, InterfaceError
from .cursor import Cursor


class Connection(object):
    def _assert_valid(self):
        if self.impl is None:
            raise InterfaceError("Connection already closed")

    def __init__(self, impl):
        self.impl = impl
        self.cursors = []

    @translate_exceptions
    def cursor(self):
        """
        Create a new ``Cursor`` instance associated with this ``Connection``
        
        :return: A new ``Cursor`` instance
        """
        self._assert_valid()
        c = Cursor(self.impl.cursor())
        self.cursors.append(c)
        return c

    @translate_exceptions
    def commit(self):
        """
        Commits the current transaction
        """
        self._assert_valid()
        self.impl.commit()

    @translate_exceptions
    def rollback(self):
        """
        Roll back all changes in the current transaction 
        """
        self._assert_valid()
        self.impl.rollback()

    def close(self):
        """
        Close the connection and all associated cursors. This will implicitly
        roll back any uncommited operations.
        """
        for c in self.cursors:
            c.close()
        self.cursors = []
        self.impl = None
